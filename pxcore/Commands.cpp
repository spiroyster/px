#include "Instance.hpp"

#include <pxlib/Include/String.hpp>
#include <pxlib/Include/FileIO.hpp>

#include <filesystem>

namespace
{
	px::String Detokenise(const std::vector<px::String>& tokens)
	{
		px::String result;
		for (unsigned int t = 0; t < tokens.size(); ++t)
			result += (tokens[t].find(' ') == std::string::npos ? tokens[t] : (px::Str("\"") + tokens[t] + px::Str("\""))) + px::Str(" ");

		if (!result.empty())
			result.pop_back();
		return result;
	}
}

PX_COMMAND(kill)
{
	// This command doesn't perform any business logic, rather dispatches kill event
	// Termination must come from client...
	px::Dispatch::Kill();
	return true;
}

PX_COMMAND(version)
{
	pxCore::GetInstance().Message(px::Context::Version().Summary());
	return true;
}

PX_COMMAND(echo)
{
	for (unsigned int t = 0; t < args.size(); ++t)
		pxCore::GetInstance().Message(args[t]);

	return true;
}

PX_COMMAND(list)
{
	if (args.empty())
	{
		// Help message...
		px::Context::Message(px::Str("List {modules} {commands} {parameters} {all}"));
		return false;
	}

	bool listModules = false, listCommands = false, listParameters = false;

	for (auto a = args.begin(); a != args.end(); ++a)
	{
		if (px::Util::StringsEqualIgnoringCase(*a, px::Str("modules")))
			listModules = true;
		if (px::Util::StringsEqualIgnoringCase(*a, px::Str("commands")))
			listCommands = true;
		if (px::Util::StringsEqualIgnoringCase(*a, px::Str("parameters")))
			listParameters = true;
		if (px::Util::StringsEqualIgnoringCase(*a, px::Str("all")))
		{
			listModules = true;
			listCommands = true;
			listParameters = true;
		}
	}

	if (listModules)
	{
		auto moduleList = pxCore::GetInstance().ModuleList();
		px::String output(px::Str(listCommands || listParameters ? "Modules: \n" : ""));
		for (auto i = moduleList.begin(); i != moduleList.end(); ++i)
			output += (px::Str("{") + i->first + px::Str("} ") + i->second + px::Str("\n"));

		pxCore::GetInstance().Message(output);
	}

	if (listCommands)
	{
		auto commandList = pxCore::GetInstance().CommandList();
		px::String output(px::Str(listModules || listParameters ? "Commands: \n" : ""));
		for (auto i = commandList.begin(); i != commandList.end(); ++i)
			output += (px::Str(" {") + (*i)->Owner() + px::Str("} ") + (*i)->Name() + px::Str("\n"));

		pxCore::GetInstance().Message(output);
	}

	if (listParameters)
	{
		px::String output(px::Str(listCommands || listModules ? "Parameters: \n" : ""));
		for (auto i = pxCore::GetInstance().GetParameters().begin(); i != pxCore::GetInstance().GetParameters().end(); ++i)
			output += (px::Str(" ") + i->first + px::Str("=") + i->second.GetAsString() + px::Str("\n"));

		pxCore::GetInstance().Message(output);
	}

	return true;
}

// install
PX_COMMAND(install)
{
	// Modules, always default look in the Application location...
	if (args.size() == 1)
	{
		px::String moduleFilename = px::Str(args.front()) + px::Str(".dll");
		std::filesystem::path moduleFilepath = moduleFilename;

		// First see if it is a absolute path...
		if (!std::filesystem::exists(moduleFilepath))
		{
			// If not check in the application path for the Core
			if (auto clientPath = px::GetParameter(pxCore::GetInstance().GetParameters(), px::KeyID::ClientPath, px::Parameter()).GetString())
				moduleFilepath = std::filesystem::path(*clientPath).parent_path() / moduleFilename;

			if (!std::filesystem::exists(moduleFilepath))
			{
				// Otherwise check in the modules path...
				if (auto modulesPath = px::GetParameter(pxCore::GetInstance().GetParameters(), px::KeyID::ModulesPath, px::Parameter()).GetString())
					moduleFilepath = std::filesystem::path(*modulesPath) / moduleFilename;
			}
		}

		if (std::filesystem::exists(moduleFilepath))
		{
			pxCore::GetInstance().ModuleInstall(moduleFilepath);
		}
		else
			throw px::Exception(px::ErrorID::ModuleLoadFail,
				{
					{ px::TagID::Command, Name() },
					{ px::TagID::Filename, moduleFilename },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});
	}
	else
		throw px::Exception(px::ErrorID::CommandSyntaxInvalid,
			{
				{ px::TagID::Command, Name() },
				{ px::TagID::Value, Detokenise(args) }
			});

	return true;
}

// uninstall
PX_COMMAND(uninstall)
{
	// there should be at least 1 args... the filename and path... (we ignore .dll part)...
	if (args.size() == 1)
	{
		std::filesystem::path module(args.front());

		// Unregister the module (this will invalidate all functors causing them to reregister next time they are called) ...
		pxCore::GetInstance().ModuleUninstall(module);

		return true;
	}
	else
		throw px::Exception(px::ErrorID::CommandSyntaxInvalid,
			{
				{ px::TagID::Command, Name() },
				{ px::TagID::Value, Detokenise(args) }
			});

	return false;
}

namespace
{
	px::String Parse(const px::String& syntax)
	{
		auto commentItr = syntax.find('#');
		if (commentItr != px::String::npos)
			return px::Util::Trim(syntax.substr(0, commentItr), syntax.substr(commentItr + 1));
		return px::Util::Trim(syntax, px::String());
	}
}

// macro
PX_COMMAND(macro)
{
	if (args.empty())
		return false;

	// Load in and run the macro...
	auto macro = px::Util::TextFile(args.front()).GetLines();
	bool haltOnError = *px::GetParameter(pxCore::GetInstance().GetParameters(), px::KeyID::MacroHaltOnError, px::Parameter(false)).GetBool();

	for (unsigned int ln = 0; ln < macro.size(); ++ln)
	{
		try
		{
			px::Command(Parse(macro[ln]));
		}
		catch (const px::Exception& e)
		{
			px::Exception e2(e.Id(), px::MergeParameters(e.Meta(),
				{
					{px::TagID::MacroLineNumber, px::Str(std::to_string(ln))},
					{px::TagID::MacroLineSyntax, px::Str(macro[ln])}
				}, true));
			
			if (haltOnError)
				throw e2;
			else
				pxCore::GetInstance().ReportException({ e2 });
		}
		catch (const std::exception& e)
		{
			px::Exception e2(e, 
				{
					{px::TagID::MacroLineNumber, px::Str(std::to_string(ln))},
					{px::TagID::MacroLineSyntax, px::Str(macro[ln])}
				});

			if (haltOnError)
				throw e2;
			else
				pxCore::GetInstance().ReportException({ e2 });
		}
		catch (...)
		{
			// Swallow...
			px::Exception e(px::ErrorID::UnknownError,
				{
					{ px::TagID::MacroLineNumber, px::Str(std::to_string(ln)) },
					{ px::TagID::MacroLineSyntax, px::Str(macro[ln]) }
				});

			if (haltOnError)
				throw e;
			else
				pxCore::GetInstance().ReportException({ e });
		}
	}


	// Load in and run the macro...
	//px::util::Macro(px::util::TextFile(args.front()).GetLines()).Run();

	return true;
}

// Replay command (takes log file and replays all command in order)
PX_COMMAND(replay)
{
	// Load the log file and extract the commands...


	// Create a macro for this and run it...

	return false;
}
