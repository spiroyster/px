#ifndef pxCore_MODULEMANAGER_HPP
#define pxCore_MODULEMANAGER_HPP

#include <include/px.hpp>

#include <memory>
#include <filesystem>

namespace pxCore
{
	class ModuleManager
	{
		class Module;
		std::list<std::shared_ptr<Module>> modules_;
	
	public:
		void InstallModule(const std::filesystem::path& moduleFilepath);
		void UnInstallModule(const px::String& moduleName);
		void UnInstallAll();
		std::map<px::String, px::String> Listing() const;
		void RefreshModules();
		void SetModuleVersion(const px::String& moduleName, const px::VersionInformation& version);

	};
}

#endif // pxCore_MODULEMANAGER_HPP