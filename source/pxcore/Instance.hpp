#ifndef PXCORE_CORE_HPP
#define PXCORE_CORE_HPP

#include <include/px/Core.hpp>

#include <map>
#include <mutex>

#include "CommandManager.hpp"
#include "ObserverManager.hpp"
#include "ModuleManager.hpp"

namespace pxCore
{
	class Instance : public px::CoreInterface
	{
		px::Parameters parameters_;

		// Global mutex...
		std::list<px::String> globalMutexOwners_;
		std::unique_lock<std::recursive_mutex> globalMutexLock_;
		std::recursive_mutex globalMutex_;

		// Managers...
		ModuleManager moduleManager_;
		ObserverManager observerManager_;
		CommandManager commandManager_;
		// task manager...

	public:

		void Initialise();
		void Terminate();

		px::VersionInformation Version();

		void ReportException(const std::vector<px::Exception>& e);
		void Message(const px::String& msg);
		px::Parameters& GetParameters();
		const px::Parameters& GetParameters() const;

		// Mutex (global)
		void GlobalLock(const px::String& owner);
		void GlobalUnlock(const px::String& owner);
		const std::list<px::String>& GlobalLockOwners() const;

		// Task manager...
		/*bool IsMainThread() const;
		unsigned int MainThreadID();
		void ProcessTasks();
		void AddTask(std::shared_ptr<Task>& task, unsigned int pollingRate);*/

		// Parameters...
		px::Parameters From(const px::String& syntax) const;
		px::String To(const px::Parameters& parameters) const;

		// Module manager...
		void ModuleInstall(const std::filesystem::path& moduleFilepath);
		void ModuleUninstall(const px::String& moduleName);
		std::map<px::String, px::String> ModuleList();
		void ModulesRefresh();
		void ModuleUninstallAll();
		void Handshake(const px::String& moduleName, const px::VersionInformation& version);

		// Command manager
		void RegisterCommand(px::CommandInterface* commandInterface);
		void UnregisterCommand(px::CommandInterface* commandInterface);
		bool CommandInvoke(const px::String& syntax) const;
		bool CommandInvoke(const px::CommandInterface::Arguments& arguments) const;
		bool CommandIsValid(const px::String& name) const;
		bool CommandIsValid(const px::CommandInterface* command) const;
		std::vector<const px::CommandInterface*> CommandList() const;
		

		// Event manager
		void RegisterObserver(px::ObserverInterface* observer);
		void UnregisterObserver(px::ObserverInterface* observer);
		void ObserverToBack(px::ObserverInterface* observer);
		void ObserverToFront(px::ObserverInterface* observer);
		std::vector<px::ObserverInterface*> Subscribers(const px::ObserverInterface::ID& eventID);

	};

	extern Instance& GetInstance();
}

#endif // PXCORE_CORE_HPP
