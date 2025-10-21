#include "ModuleManager.hpp"

#include "Instance.hpp"

#include <Windows.h>

namespace pxCore
{
	class ModuleManager::Module
	{
	public:

		Module(const std::filesystem::path& moduleFilepath)
			:	moduleFilepath_(moduleFilepath)
		{
			hInstance_ = LoadLibrary(moduleFilepath.c_str());
			if (!hInstance_)
				throw px::Exception(px::ErrorID::ModuleLoadFail,
					{
						{ px::TagID::Filename, px::Str(moduleFilepath.string())}
					});

			Initialise();
		}

		~Module()
		{
			// Call kill
			if (pxModuleKillFunc pxmkf = (pxModuleKillFunc)GetProcAddress(hInstance_, pxModuleKillName.c_str()))
				pxmkf();

			FreeLibrary(hInstance_);
		}

		/// <summary>
		/// The filename and extension of the module.
		/// </summary>
		/// <returns>The name of the module</returns>
		std::string GetModuleName() const
		{
			return moduleFilepath_.stem().string();
		}

		/// <summary>
		/// The module filename and path.
		/// </summary>
		/// <returns>The module filename and path</returns>
		const std::filesystem::path& GetModuleFilepath() const { return moduleFilepath_; }

		void Initialise()
		{
			// Call main
			if (pxModuleMainFunc pxmmf = (pxModuleMainFunc)GetProcAddress(hInstance_, pxModuleMainName.c_str()))
			{
				auto& inst = GetInstance();
				if (!pxmmf(&inst))
				{
					throw px::Exception(px::ErrorID::ModuleLoadFail,
						{
							{ px::TagID::Callee, px::Str(__FUNCTION__)},
							{ px::TagID::Filename, px::Str(moduleFilepath_.string()) },
							{ px::TagID::Function, px::Str("Handshake") }
						});
				}
			}
			else
			{
				throw px::Exception(px::ErrorID::ModuleLoadFail,
					{
						{ px::TagID::Callee, px::Str(__FUNCTION__)},
						{ px::TagID::Filename, px::Str(moduleFilepath_.string()) },
						{ px::TagID::Function, px::Str(pxModuleMainName) }
					});
			}
		}

		const px::VersionInformation& GetVersion() { return version_; }
		void SetVersion(const px::VersionInformation& version) { version_ = version; }

	private:
		HINSTANCE hInstance_;
		std::filesystem::path moduleFilepath_;
		px::VersionInformation version_;
	};

	void ModuleManager::InstallModule(const std::filesystem::path& moduleFilepath)
	{
		// We want to retain the order of installed modules since the functions and commands will be Registered in that order...
		auto itr = std::find_if(modules_.begin(), modules_.end(), [&moduleFilepath](const std::shared_ptr<Module>& mod) { return mod->GetModuleName() == moduleFilepath; });
		if (itr != modules_.end())
			throw px::Exception(px::ErrorID::ModuleAlreadyRegistered,
				{
					{ px::TagID::Filename, px::Str(moduleFilepath.string()) },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		// Create our module context...
		modules_.emplace_back(std::make_shared<Module>(moduleFilepath));
	}

	void ModuleManager::UnInstallModule(const px::String& moduleName)
	{
		// First find the module...
		auto itr = std::find_if(modules_.begin(), modules_.end(), [&moduleName](const std::shared_ptr<Module>& mod) 
			{ 
				return px::Str(mod->GetModuleName()) == moduleName; 
			});

		if (itr == modules_.end())
			throw px::Exception(px::ErrorID::ModuleNotFound,
				{
					{ px::TagID::Filename, moduleName },
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		modules_.erase(itr);
	}

	std::map<px::String, px::String> ModuleManager::Listing() const
	{
		std::map<px::String, px::String> listing;
		for (auto itr = modules_.begin(); itr != modules_.end(); ++itr)
			listing[px::Str((*itr)->GetModuleName())] = (*itr)->GetVersion().Summary();
		return listing;
	}

	void ModuleManager::UnInstallAll()
	{
		modules_.clear();
	}

	void ModuleManager::RefreshModules()
	{
		for (auto mh = modules_.begin(); mh != modules_.end(); ++mh)
			(*mh)->Initialise();
	}

	void ModuleManager::SetModuleVersion(const px::String& moduleName, const px::VersionInformation& version)
	{
		auto itr = std::find_if(modules_.begin(), modules_.end(), [&moduleName](const std::shared_ptr<Module>& mod)
			{
				return px::Str(mod->GetModuleName()) == moduleName;
			});

		if (itr == modules_.end())
			throw px::Exception(px::ErrorID::ModuleNotFound,
				{
					{ px::TagID::Filename, moduleName },
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		modules_.erase(itr);
	}
}
