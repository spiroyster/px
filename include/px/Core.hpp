#ifndef PX_COREINTERFACE_HPP
#define PX_COREINTERFACE_HPP

#include "Type.hpp"
#include "Version.hpp"
#include "Command.hpp"
#include "Event.hpp"
#include "Exception.hpp"
#include "Task.hpp"
#include "Data.hpp"
#include "Draw.hpp"

#include <vector>
#include <list>

namespace px
{

	class CoreInterface
	{
	public:
		virtual VersionInformation Version() = 0;

		virtual void ReportException(const std::vector<Exception>& e) = 0;
		virtual void Message(const String& msg) = 0;
		virtual Parameters& GetParameters() = 0;
		virtual const Parameters& GetParameters() const = 0;

		// Mutex (global)
		virtual void GlobalLock(const String& owner) = 0;
		virtual void GlobalUnlock(const String& owner) = 0;
		virtual const std::list<String>& GlobalLockOwners() const = 0;

		// Task manager...
		virtual bool IsMainThread() const = 0;
		virtual unsigned int MainThreadID() = 0;
		virtual void ProcessTasks() = 0;
		virtual void AddTask(std::shared_ptr<Task>& task, unsigned int pollingRate) = 0;

		// Parameters...
		virtual Parameters From(const String& syntax) const = 0;
		virtual String To(const Parameters& parameters) const = 0;

		// Handshake
		virtual void Handshake(const String& moduleName, const VersionInformation& version) = 0;

		// Command manager
		virtual void RegisterCommand(CommandInterface* commandInterface) = 0;
		virtual void UnregisterCommand(CommandInterface* commandInterface) = 0;
		virtual bool CommandInvoke(const String& syntax) const = 0;
		virtual bool CommandInvoke(const CommandInterface::Arguments& arguments) const = 0;
		virtual bool CommandIsValid(const String& name) const = 0;
		virtual bool CommandIsValid(const CommandInterface* command) const = 0;

		// Event manager
		virtual void RegisterObserver(ObserverInterface* observer) = 0;
		virtual void UnregisterObserver(ObserverInterface* observer) = 0;
		virtual void ObserverToBack(ObserverInterface* observer) = 0;
		virtual void ObserverToFront(ObserverInterface* observer) = 0;
		virtual std::vector<ObserverInterface*> Subscribers(const ObserverInterface::ID& eventID) = 0;

		// Project Manager...
		virtual void AddDataModel(const std::shared_ptr<DataModel>& project) = 0;
		virtual void RemoveDataModel(const String& projectName) = 0;
		virtual std::shared_ptr<DataModel> LockDataModel(const String& projectName) const = 0;
		virtual void UnlockDataModel(const String& projectName) const = 0;
		virtual std::list<String> DataModels() const = 0;
		
		// Draw
		virtual void RegisterDrawObject(px::DrawObject* drawObject) = 0;
		virtual void UnregisterDrawObject(px::DrawObject* drawObject) = 0;
		virtual std::vector<px::DrawObject*> DrawObjects() const = 0;
		virtual void RegisterTexture(px::Texture* texture) = 0;
		virtual void UnregisterTexture(px::Texture* texture) = 0;
		virtual std::vector<px::Texture*> Textures() const = 0;
		virtual void RegisterView(px::View* view) = 0;
		virtual void UnregisterView(px::View* view) = 0;
		virtual std::vector<px::View*> Views() const = 0;


		// Texture manager...

		// DrawObject manager...
	};
}

#endif PX_COREINTERFACE_HPP