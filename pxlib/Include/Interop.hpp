#ifndef AxW_SYSTEM_INTEROP_HPP
#define AxW_SYSTEM_INTEROP_HPP

#include <string>

/// <summary>
/// Helper structure for string interop. Interop Strings pass raw buffer addresses of strings and so need copying and consuming within each execution boundary they are passed between. 
/// When passing across execution boundaries, be mindful of not dangling this pointer after returning it. The buffer must exist beyond the callee function going out of scope and so 
/// usually a global static stirng is used to hold the return string, and the Interop object simply wraps with the size and raw pointer information. The caller will (hopefully) copy 
/// and begin managing it's own version pretty soon after getting the InteropString returned to them (just make sure no concurrent process is calling this function).
/// </summary>
struct InteropString
{
	unsigned int size_;	///< The size of the char array.
	const wchar_t* str_;	///< Raw pointer of the char array in the other execution boundary memory.

	/// <summary>
	/// Convert an (unmanged) Interop string to a (managed) std::Stringing. 
	/// </summary>
	/// <returns>A (managed) std string of the (unmanged) interop string.</returns>
	std::wstring ToStdString()
	{
		if (!size_)
			return std::wstring();
		else
			return std::wstring(str_, size_);
	}
};

typedef void(__stdcall* InitialiseMessageCallback)(const wchar_t* msg);
typedef void(__stdcall* InitialiseExceptionCallback)(unsigned int exceptionID, const wchar_t* msg);

// Function prototypes...

/// <summary>
/// AxW module exported function prototype for the module init/main entry point. This is called when a module is installed.
/// </summary>
typedef bool(__cdecl* pxModuleMainFunc)(void*);
static const std::string pxModuleMainName("pxModuleMain");

/// <summary>
/// AxW module exported function prototype for the module kill entry point. This is called when a module is uninstalled.
/// </summary>
typedef void(__cdecl* pxModuleKillFunc)(void);
static const std::string pxModuleKillName("pxModuleKill");


// Initialise
typedef void* (__cdecl* pxCoreInitFunc)(InitialiseMessageCallback msgCallback, InitialiseExceptionCallback exceptionCallback, InteropString parameters);
static const std::string pxCoreInitName("pxCoreInit");

// Kill
typedef void(__cdecl* pxCoreKillFunc)(InteropString killParam);
static const std::string pxCoreKillName("pxCoreKill");

// Idle
typedef void(__cdecl* pxCoreIdleFunc)();
static const std::string pxCoreIdleName("pxCoreIdle");



#endif // AxW_SYSTEM_INTEROP_HPP