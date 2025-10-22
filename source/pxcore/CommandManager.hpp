#ifndef pxCore_COMMANDMANAGER_HPP
#define pxCore_COMMANDMANAGER_HPP

#include <include/px.hpp>

namespace pxCore
{
	class CommandManager
	{
		std::map<px::String, std::list<px::CommandInterface*>> commands_;

	public:

		void RegisterCommand(px::CommandInterface* commandInterface);
		void UnregisterCommand(px::CommandInterface* commandInterface);
		bool CommandInvoke(const px::String& syntax) const;
		bool CommandInvoke(const px::CommandInterface::Arguments& arguments) const;
		bool CommandIsValid(const px::String& name) const;
		bool CommandIsValid(const px::CommandInterface* command) const;
		const std::map<px::String, std::list<px::CommandInterface*>>& CommandList() const { return commands_; }
	};
}

#endif // pxCore_COMMANDMANAGER_HPP