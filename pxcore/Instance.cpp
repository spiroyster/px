#include "instance.hpp"

#include <pxlib/px.hpp>

#include <sstream>
#define RAPIDJSON_HAS_STDSTRING 1
#include <thirdparty/rapidjson/include/rapidjson/document.h>
#include <thirdparty/rapidjson/include/rapidjson/prettywriter.h>
#include <thirdparty/rapidjson/include/rapidjson/stringbuffer.h>

namespace pxCore
{
	namespace 
	{
		std::wstring ReplaceAll(const std::wstring& str, const std::wstring& what, const std::wstring& with)
		{
			std::wstring result = str;
			auto itr = result.find(what);
			while (itr != std::wstring::npos)
			{
				result.replace(itr, what.size(), with);
				itr = result.find(what, itr + 1);
			}
			return result;
		}

	}

	px::VersionInformation Instance::Version()
	{
		return px::Context::Version();
	}

	void Instance::Initialise()
	{

	}

	void Instance::Terminate()
	{
		// We need to systematically destroy managed objects...
		Message(px::Str("Have a nice day!"));

		// Remove all the modules (this should destruct all of the modules observers and commands and unregister them)
		moduleManager_.UnInstallAll();

	}

	void Instance::ReportException(const std::vector<px::Exception>& e)
	{
		px::Dispatch::Exception(e);
	}

	void Instance::Message(const px::String& msg)
	{
		px::Dispatch::Message(msg);
	}

	px::Parameters& Instance::GetParameters()
	{
		return parameters_;
	}

	const px::Parameters& Instance::GetParameters() const
	{
		return parameters_;
	}

	// Mutex (global)
	void Instance::GlobalLock(const px::String& owner)
	{
		globalMutex_.lock();
		globalMutexOwners_.emplace_back(owner);
	}

	void Instance::GlobalUnlock(const px::String& owner)
	{
		globalMutex_.unlock();

		// Remove this owner...
		auto itr = std::find_if(globalMutexOwners_.begin(), globalMutexOwners_.end(), [&owner](const px::String& globalOwner) { return globalOwner == owner; });
		if (itr != globalMutexOwners_.end())
			globalMutexOwners_.erase(itr);
	}

	const std::list<px::String>& Instance::GlobalLockOwners() const
	{
		return globalMutexOwners_;
	}

	/*bool IsMainThread() const;
	unsigned int MainThreadID();
	void ProcessTasks();
	void AddTask(std::shared_ptr<Task>& task, unsigned int pollingRate);*/

	void Instance::Handshake(const px::String& moduleName, const px::VersionInformation& version)
	{
		moduleManager_.SetModuleVersion(moduleName, version);
	}

	// Command manager
	void Instance::RegisterCommand(px::CommandInterface* commandInterface)
	{
		commandManager_.RegisterCommand(commandInterface);
	}

	void Instance::UnregisterCommand(px::CommandInterface* commandInterface)
	{
		commandManager_.UnregisterCommand(commandInterface);
	}

	bool Instance::CommandInvoke(const px::String& syntax) const
	{
		return commandManager_.CommandInvoke(syntax);
	}

	bool Instance::CommandInvoke(const px::CommandInterface::Arguments& arguments) const
	{
		return commandManager_.CommandInvoke(arguments);
	}

	bool Instance::CommandIsValid(const px::String& name) const
	{
		return commandManager_.CommandIsValid(name);
	}

	bool Instance::CommandIsValid(const px::CommandInterface* command) const
	{
		return commandManager_.CommandIsValid(command);
	}

	std::vector<const px::CommandInterface*> Instance::CommandList() const
	{
		std::list<const px::CommandInterface*> commands;
		for (auto itr = commandManager_.CommandList().begin(); itr != commandManager_.CommandList().end(); ++itr)
			commands.emplace_back(itr->second.back());

		return { commands.begin(), commands.end() };
	}

	// Event manager
	void Instance::RegisterObserver(px::ObserverInterface* observer)
	{
		observerManager_.RegisterObserver(observer);
	}

	void Instance::UnregisterObserver(px::ObserverInterface* observer)
	{
		observerManager_.UnregisterObserver(observer);
	}

	void Instance::ObserverToBack(px::ObserverInterface* observer)
	{
		observerManager_.ObserverToBack(observer);
	}

	void Instance::ObserverToFront(px::ObserverInterface* observer)
	{
		observerManager_.ObserverToFront(observer);
	}

	std::vector<px::ObserverInterface*> Instance::Subscribers(const px::ObserverInterface::ID& observer)
	{
		return observerManager_.Subscribers(observer);
	}

	void Instance::ModuleInstall(const std::filesystem::path& moduleFilepath)
	{
		moduleManager_.InstallModule(moduleFilepath);
	}

	void Instance::ModuleUninstall(const px::String& moduleName)
	{
		moduleManager_.UnInstallModule(moduleName);
	}

	std::map<px::String, px::String> Instance::ModuleList()
	{
		return moduleManager_.Listing();
	}

	void Instance::ModulesRefresh()
	{
		moduleManager_.RefreshModules();
	}

	void Instance::ModuleUninstallAll()
	{
		moduleManager_.UnInstallAll();
	}

	// Parameters...
	px::Parameters Instance::From(const px::String& syntax) const
	{
		// Use rapidjson to parse the syntax...
		if (syntax.empty())
			return px::Parameters();

		rapidjson::GenericStringStream<rapidjson::UTF16<>> ss(syntax.c_str());
		rapidjson::Document document;

		document.ParseStream(ss);

		px::Parameters result;

		rapidjson::ParseErrorCode err = document.GetParseError();
		if (err)
			throw px::Exception(px::ErrorID::ParametersSyntax, 
				{
					{ px::TagID::What, px::Str(std::to_string(err))},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		assert(document.IsObject());

		for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
		{
			px::String key = px::Str(itr->name.GetString());

			if (itr->value.IsBool())
				result[key] = px::Parameter(itr->value.GetBool());
			else if (itr->value.IsFloat())
				result[key] = px::Parameter(itr->value.GetFloat());
			else if (itr->value.IsInt())
				result[key] = px::Parameter(itr->value.GetInt());
			else if (itr->value.IsString())
				result[key] = px::Parameter(px::Str(itr->value.GetString()));
		}

		return result;
	}

	px::String Instance::To(const px::Parameters& parameters) const
	{
		// Create json...
		if (parameters.empty())
			return L"";

		std::wstringstream oss;
		oss << "{ ";
		for (auto p = parameters.begin(); p != parameters.end(); ++p)
		{
			oss << "\"" << p->first << "\": ";
			if (auto floatVal = p->second.GetFloat())
				oss << std::to_wstring(*floatVal);
			else if (auto intVal = p->second.GetInt())
				oss << std::to_wstring(*intVal);
			else if (auto boolVal = p->second.GetBool())
			{
				if (*boolVal)
					oss << "true";
				else
					oss << "false";
			}
			else if (auto stringVal = p->second.GetString())
			{
				// Replace all the '\\' with '/'...
				*stringVal = ReplaceAll(*stringVal, L"\\", L"/");
				oss << "\"" << *stringVal << "\"";
			}
			oss << ", ";
		}
		px::String result = oss.str();
		result.pop_back();
		result.pop_back();
		result.append(L" }");
		return result;
	}

}