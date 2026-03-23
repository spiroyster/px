#include "instance.hpp"

#include <include/px.hpp>

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
		mainThreadID_ = std::this_thread::get_id();
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

	unsigned int Instance::MainThreadID()
	{
		return static_cast<unsigned int>(std::hash<std::thread::id>{}(mainThreadID_));
	}

	bool Instance::IsMainThread() const
	{
		return std::this_thread::get_id() == mainThreadID_;
	}

	void Instance::ProcessTasks()
	{
		// Only process/poll tasks if they are called from main thread...
		if (!IsMainThread())
			return;

		std::list<std::shared_ptr<px::Task>> erroredTasks;

		for (auto task = tasks_.begin(); task != tasks_.end();)
		{
			auto t = (*task)->Poll();

			try
			{
				// Poll the task...
				switch (t)
				{
				case px::Task::TaskPolled:
					(*task)->Step();
					break;
				case px::Task::TaskFinished:
					px::Message(px::Str("(" + std::to_string(MainThreadID()) + ")-> Finishing task " + std::to_string((*task)->ThreadID()) + "."));
					(*task)->Finished();
					break;
				default:
					break;
				}
			}
			catch (const px::Exception& e)
			{
				(*task)->SetError(e);
				t = px::Task::TaskErrorer;
			}

			// If task has finished or errorer, remove it from the list...
			if (t == px::Task::TaskFinished || t == px::Task::TaskErrorer)
			{
				px::Message(px::Str("(" + std::to_string(MainThreadID()) + ")-> Destroying task " + std::to_string((*task)->ThreadID()) + "."));
				px::Dispatch::TaskFinish((*task)->ThreadID());

				//if (t == AxW::System::Task::TaskErrorer)
				if (t == px::Task::TaskErrorer)
					(*task)->Errored(*(*task)->GetError());

				// Remove the task...
				task = tasks_.erase(task);

				if (tasks_.empty())
					px::Dispatch::ClientEndIdle();

				px::ReportException({});
			}
			else
				++task;
		}
	}

	void Instance::AddTask(std::shared_ptr<px::Task>& task, unsigned int pollingRate)
	{
		// Only add tasks if they were created in this thread...
		if (!IsMainThread())
			throw px::Exception(px::ErrorID::TaskInitFail,
				{
					{ px::TagID::What, px::Str("Task can only be started from main thread") },
					{ px::TagID::Expected, px::Str(std::to_string(MainThreadID())) },
					{ px::TagID::Result, px::Str(std::to_string(static_cast<unsigned int>(std::hash<std::thread::id>{}(std::this_thread::get_id())))) },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		// Set the polling rate of the task...
		task->PollingRate(pollingRate);
		tasks_.emplace_back(task);

		// Start the task!!!
		task->Run();

		px::Message(px::Str("(" + std::to_string(MainThreadID()) + ")-> Starting task " + std::to_string(task->ThreadID()) + " @" + std::to_string(pollingRate) + " ms."));
		px::Dispatch::TaskStart(task->ThreadID());
	}

	class Instance::DataModelHandle
	{
		// the project...
		std::shared_ptr<px::DataModel> dataModel_;

		// mutex for this project... think this needs to be 
		//std::timed_mutex mutex_;
		std::unique_lock<std::recursive_mutex> lock_;
		std::recursive_mutex mutex_;

	public:
		DataModelHandle(const std::shared_ptr<px::DataModel>& dataModel)
			: dataModel_(dataModel), lock_(mutex_, std::defer_lock)
		{
			//AxW::Message(AxW::Str("[" + ThreadID() + "] constructing handle ") + project_->Name());
		}

		// Request this project...
		std::shared_ptr<px::DataModel> Get()
		{
			// try to lock this mutex...
			//if (lock_.try_lock_for(200ms))
			//	return project_;
			//AxW::Message(AxW::Str("[" + ThreadID() + "] locking ") + project_->Name());

			mutex_.lock();

			//lock_.lock();
			return dataModel_;

			/*throw AxW::Exception(AxW::Error::ID::SessionProjectTimeout,
				{
					{ AxW::Error::Tag::Project, project_->Name() },
					{ AxW::Error::Tag::Callee, AxW::Str(__FUNCTION__) }
				});*/
		}

		void Unlock()
		{
			//AxW::Message(AxW::Str("[" + ThreadID() + "] unlocking ") + project_->Name());
			mutex_.unlock();
			//lock_.unlock();
		}

		const px::String& Name() const { return dataModel_->Name(); }

	};

	void Instance::AddDataModel(const std::shared_ptr<px::DataModel>& project)
	{
		// find this project...
		auto dataModelItr = std::find_if(dataModels_.begin(), dataModels_.end(), [&project](const std::shared_ptr<DataModelHandle>& p) { return p->Name() == project->Name(); });

		if (dataModelItr != dataModels_.end())
			throw px::Exception(px::ErrorID::DataModelAlreadyExists,
				{
					{ px::TagID::Name, project->Name() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});
			
		// Add the data model and start managing...
		dataModels_.emplace_back(std::make_shared<DataModelHandle>(project));
		px::Dispatch::DataModelAdded(project->Name());
	}

	void Instance::RemoveDataModel(const px::String& projectName)
	{
		auto dataModelItr = std::find_if(dataModels_.begin(), dataModels_.end(), [&projectName](const std::shared_ptr<DataModelHandle>& p) { return p->Name() == projectName; });

		if (dataModelItr != dataModels_.end())
			throw px::Exception(px::ErrorID::DataModelNotFound,
				{
					{ px::TagID::Name, projectName },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		// Add the data model and start managing...
		dataModels_.erase(dataModelItr);
		px::Dispatch::DataModelRemoved(projectName);
	}

	std::shared_ptr<px::DataModel> Instance::LockDataModel(const px::String& projectName) const
	{
		auto dataModelItr = std::find_if(dataModels_.begin(), dataModels_.end(), [&projectName](const std::shared_ptr<DataModelHandle>& p) { return p->Name() == projectName; });

		if (dataModelItr == dataModels_.end())
			throw px::Exception(px::ErrorID::DataModelNotFound,
				{
					{ px::TagID::Name, projectName },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});
		
		return (*dataModelItr)->Get();
	}

	void Instance::UnlockDataModel(const px::String& projectName) const
	{
		auto dataModelItr = std::find_if(dataModels_.begin(), dataModels_.end(), [&projectName](const std::shared_ptr<DataModelHandle>& p) { return p->Name() == projectName; });

		if (dataModelItr != dataModels_.end())
			(*dataModelItr)->Unlock();
	}

	std::list<px::String> Instance::DataModels() const
	{
		std::list<px::String> result;

		for (auto itr = dataModels_.begin(); itr != dataModels_.end(); ++itr)
			result.emplace_back((*itr)->Name());

		return result;
	}

	void Instance::RegisterDrawObject(px::DrawObject* drawObject)
	{
		drawManager_.RegisterDrawObject(drawObject);
	}
	void Instance::UnregisterDrawObject(px::DrawObject* drawObject)
	{
		drawManager_.UnregisterDrawObject(drawObject);
	}
	std::vector<px::DrawObject*> Instance::DrawObjects() const
	{
		const auto& drawObjects = drawManager_.DrawObjects();
		std::vector<px::DrawObject*> result(drawObjects.size());
		
		auto s = drawObjects.begin();
		auto d = result.begin();
		for (; s != drawObjects.end() && d != result.end(); ++s, ++d)
			*d = s->second;

		return result;
	}
	
	void Instance::RegisterTexture(px::Texture* texture)
	{
		drawManager_.RegisterTexture(texture);
	}
	
	void Instance::UnregisterTexture(px::Texture* texture)
	{
		drawManager_.UnregisterTexture(texture);
	}
	
	std::vector<px::Texture*> Instance::Textures() const
	{
		const auto& textures = drawManager_.Textures();
		std::vector<px::Texture*> result(textures.size());

		auto s = textures.begin();
		auto d = result.begin();
		for (; s != textures.end() && d != result.end(); ++s, ++d)
			*d = s->second;

		return result;
	}
	
	void Instance::RegisterView(px::View* view)
	{
		drawManager_.RegisterView(view);
	}
	
	void Instance::UnregisterView(px::View* view)
	{
		drawManager_.UnregisterView(view);
	}
	
	std::vector<px::View*> Instance::Views() const
	{
		const auto& views = drawManager_.Views();
		std::vector<px::View*> result(views.size());

		auto s = views.begin();
		auto d = result.begin();
		for (; s != views.end() && d != result.end(); ++s, ++d)
			*d = s->second;

		return result;
	}
}