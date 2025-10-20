#ifndef PX_VERSION_HPP
#define PX_VERSION_HPP

#include "type.hpp"

namespace px
{
	/// <summary>
	/// Accessor wrapper for version information for this API. Version information is accessable via this classes methods.
	/// </summary>
	class VersionInformation
	{
		unsigned int major_, minor_, build_;
		String information_, time_, date_;

	public:
		VersionInformation();

		/// <summary>
		/// Get the Major version number of the API in use. Major version changes are breaking and API's with a different major version number to the core/instance are not expected to work and
		/// so are prevented from doing so. This can also include major functionality/behavioural change of the API (even if interfaces don't).
		/// </summary>
		/// <returns>The major version number.</returns>
		unsigned int Major() const { return major_; }

		/// <summary>
		/// Get the Minor version number of the API in use. Minor version changes are not breaking, and should indicate an interface addition or implementation change.
		/// </summary>
		/// <returns>The minor number of the API in use.</returns>
		unsigned int Minor() const { return minor_; }

		/// <summary>
		/// Get the Build number of the API in use.
		/// </summary>
		/// <returns>The build number of the API in use</returns>
		unsigned int BuildNumber() const { return build_; }

		String BuildInformation() const { return information_; }

		/// <summary>
		/// Get the time this API binary was built.
		/// </summary>
		/// <returns>The time this API binary was built.</returns>
		String BuildTime() const { return time_; }

		/// <summary>
		/// Get the date this API binary was built.
		/// </summary>
		/// <returns>The date this API binary was built.</returns>
		String BuildDate() const { return date_; }

		String Summary() const;
	};
}

#endif // PX_VERSION_HPP