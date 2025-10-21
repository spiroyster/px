#include "CommandManager.hpp"

#include <pxlib/px.hpp>
#include <pxlib/Include/String.hpp>

namespace pxCore
{
	namespace
	{
		std::list<px::String> Tokenise(const px::String& syntax)
		{
			std::list<px::String> tokens(1);

			bool quoting = false;
			bool escaping = false;

			for (unsigned int c = 0; c < syntax.size(); ++c)
			{
				auto ch = syntax[c];
				switch (ch)
				{

					/*case '\\':
					{
						escaping = true;
						break;
					}*/

				case L' ':
				{
					if (quoting)
						tokens.back() += ch;
					else
						tokens.emplace_back(px::String());
					break;
				}

				case L'\"':
				{
					if (escaping)
					{
						tokens.back() += ch;
						escaping = false;
					}
					else
					{
						if (quoting)
						{
							quoting = false;
							tokens.emplace_back(px::String());
						}
						else
							quoting = true;
					}
					break;
				}

				default:
					tokens.back() += ch;
					break;
				}
			}

			// Remove empty tokens or tokens with only whitespace...
			for (auto tok = tokens.begin(); tok != tokens.end();)
			{
				if (tok->empty() || px::Util::IsWhiteSpace(*tok))
					tok = tokens.erase(tok);
				else
					++tok;
			}

			return tokens;
		}
		
	}

	// Command manager
	void CommandManager::RegisterCommand(px::CommandInterface* commandInterface)
	{
		// First find the command (if any)...
		auto cmd = commands_.find(commandInterface->Name());
		if (cmd != commands_.end())
		{
			// Check if this command is already registered...
			auto itr = std::find_if(cmd->second.begin(), cmd->second.end(), [&commandInterface](const px::CommandInterface* c) { return c->Owner() == commandInterface->Owner(); });
			if (itr != cmd->second.end())
				throw px::Exception(px::ErrorID::CommandAlreadyRegistered,
					{
						{ px::TagID::Command, commandInterface->Name()},
						{ px::TagID::Owner, commandInterface->Owner()},
						{ px::TagID::Callee, px::Str(__FUNCTION__)},
					});
		}

		commands_[commandInterface->Name()].emplace_back(commandInterface);
	}

	void CommandManager::UnregisterCommand(px::CommandInterface* commandInterface)
	{
		auto cmd = commands_.find(commandInterface->Name());
		if (cmd != commands_.end())
		{
			// find the correct owner...
			auto itr = std::find_if(cmd->second.begin(), cmd->second.end(), [&commandInterface](const px::CommandInterface* c) { return c->Owner() == commandInterface->Owner(); });
			if (itr != cmd->second.end())
				cmd->second.erase(itr);
		}

		// If there are no more commands in the list...
		if (cmd->second.empty())
			commands_.erase(cmd);
	}

	bool CommandManager::CommandInvoke(const px::String& syntax) const
	{
		// First tokenise...
		auto tokens = Tokenise(syntax);
		return CommandInvoke(std::vector<px::String>{tokens.begin(), tokens.end()});
	}

	bool CommandManager::CommandInvoke(const px::CommandInterface::Arguments& arguments) const
	{
		px::Dispatch::CommandInvoke(arguments);

		if (arguments.empty())
			return false;

		auto itr = commands_.find(arguments.front());
		if (itr == commands_.end())
			throw px::Exception(px::ErrorID::CommandNotFound,
				{
					{ px::TagID::Command, arguments.front()},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		auto args = px::CommandInterface::Arguments{ arguments.begin() + 1, arguments.end() };

		bool result = itr->second.back()->Invoke(args);
		if (result)
			px::Dispatch::CommandSucceed(args);
		else
			px::Dispatch::CommandFail(arguments);
		return result;
	}

	bool CommandManager::CommandIsValid(const px::String& name) const
	{
		return commands_.find(name) != commands_.end();
	}

	bool CommandManager::CommandIsValid(const px::CommandInterface* command) const
	{
		return CommandIsValid(command->Name());
	}
}