#include <include/px.hpp>
#include <include/px/Core.hpp>
#include <include/px/Interop.hpp>

#include <filesystem>

#ifdef PX_WIN
#include <Windows.h>
#include <codecvt>
#include <locale>
#endif // PX_WIN

#ifdef PX_NIX

#endif // PX_NIX

#include <sstream>

namespace px
{
	namespace CurrentVersion
	{
		static unsigned int Major = 0;
		static unsigned int Minor = 1;
		static unsigned int Build = 1;
		static const String Information = Str("Internal Development Version");
		static const String Date = Str(__DATE__);
		static const String Time = Str(__TIME__);
	}
	
	namespace
	{
		static std::filesystem::path pxModuleFilename;
		static const std::wstring pxCoreFilename(L"pxcore.dll");

		static void* handle = nullptr;
		static CoreInterface* coreInstance = nullptr;
		static pxCoreIdleFunc coreIdleFunc = nullptr;
		static bool coreConnected = false;

		void AssignModuleFilepath()
		{
			wchar_t path[MAX_PATH];
			HMODULE hm = NULL;

			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)pxModuleMainName.c_str(), &hm);

			// first see if this is a dll...
			DWORD size = GetModuleFileName(hm, path, MAX_PATH);

			// If not, this must be exe...
			if (!size)
				size = GetModuleFileName(NULL, path, MAX_PATH);

			pxModuleFilename = std::filesystem::path(std::wstring(path, size));
		}

		std::string WideToANSI(const std::wstring& wstr)
		{
			int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
			std::string str(count, 0);
			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
			return str;
		}

		std::wstring AnsiToWide(const std::string& str)
		{
			int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
			std::wstring wstr(count, 0);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
			return wstr;
		}

		String ReplaceAll(const String& str, const String& what, const String& with)
		{
			String result = str;
			auto itr = result.find(what);
			while (itr != String::npos)
			{
				result.replace(itr, what.size(), with);
				itr = result.find(what, itr + 1);
			}
			return result;
		}

		CoreInterface& GetCoreInstance()
		{
			if (!coreInstance)
				throw Exception(ErrorID::InstanceInvalid);
			return *coreInstance;
		}

		// The maximum number of commands this module is allowed to have...
		static const int MaxNumberOfCommandsPerModule = 100;

		// A temp holding location for registered commands (copied to instance upon handshake)
		static CommandInterface* moduleCommands[MaxNumberOfCommandsPerModule];

		void RegisterCommands()
		{
			for (unsigned int ii = 0; ii < MaxNumberOfCommandsPerModule; ++ii)
			{
				if (!moduleCommands[ii])
					continue;

				GetCoreInstance().RegisterCommand(moduleCommands[ii]);

				// Clear the tmp buffer...
				moduleCommands[ii] = nullptr;
			}
		}

		void RegisterCommand(CommandInterface* command)
		{
			// Since commands can be created upon instantiation, we need to cache the commands and then only register them when instructed (RegisterCommands)

			// first check if this command has been registered...
			CommandInterface* itr = nullptr;

			for (unsigned int a = 0; a < MaxNumberOfCommandsPerModule; ++a)
			{
				if (moduleCommands[a])
					if (moduleCommands[a]->Name() == command->Name())
					{
						moduleCommands[a] = command;
						itr = moduleCommands[a];
					}
			}

			if (!itr)
			{
				// find the next avaliable...
				for (unsigned int a = 0; a < MaxNumberOfCommandsPerModule && !itr; ++a)
				{
					if (!moduleCommands[a])
					{
						moduleCommands[a] = command;
						itr = moduleCommands[a];
					}
				}
			}
		}
		
	}

	String Str(const std::string& s)
	{
		return AnsiToWide(s);
	}

	String Str(const std::wstring& s)
	{
		return s;
	}

	std::string ToStr(const String& str) { return WideToANSI(str); }

	// Exception...
	Exception::Exception()
		: id_(ErrorID::UnknownError)
	{
		//meta_[TagID::Thread] = Str(std::to_string(static_cast<unsigned int>(std::hash<std::thread::id>{}(std::this_thread::get_id()))));
		meta_[TagID::Offender] = Context::Name();
		meta_[TagID::Error] = id_;
	}

	Exception::Exception(const Exception::ID& id, const Parameters& meta)
		: id_(id), meta_(meta)
	{
		//meta_[TagID::Thread] = Str(std::to_string(static_cast<unsigned int>(std::hash<std::thread::id>{}(std::this_thread::get_id()))));
		meta_[TagID::Offender] = Context::Name();
		meta_[TagID::Error] = id_;
	}

	Exception::Exception(const std::exception& e, const Parameters& meta)
		: id_(ErrorID::StdException), meta_(meta)
	{
		//meta_[TagID::Thread] = Str(std::to_string(static_cast<unsigned int>(std::hash<std::thread::id>{}(std::this_thread::get_id()))));
		meta_[TagID::Offender] = Context::Name();
		meta_[TagID::Error] = id_;
	}

	// Version...
	VersionInformation::VersionInformation()
		: major_(CurrentVersion::Major),
		minor_(CurrentVersion::Minor),
		build_(CurrentVersion::Build),
		information_(CurrentVersion::Information),
		date_(CurrentVersion::Date),
		time_(CurrentVersion::Time)
	{
	}

	String VersionInformation::Summary() const
	{
		return Str(std::to_string(Major()) + "." +
			std::to_string(Minor()) + "." +
			std::to_string(BuildNumber()) + " ") +
			BuildInformation() + Str(" ") +
			BuildDate() + Str(" ") +
			BuildTime();
	}

	VersionInformation Context::Version() { return VersionInformation(); }

	// Handle...
	Context::Context(const Parameters& params, InitialiseMessageCallback msg)
	{
		// Start by loading in the library...
		handle = static_cast<void*>(LoadLibrary(pxCoreFilename.c_str()));
		if (!handle)
			throw Exception(ErrorID::HandleError,
				{
					{ TagID::Filename, pxCoreFilename }
				});

		// Merge version params with client supplied init params. This will overwrite and version info the client has supplied.
		auto paramsString = To(MergeParameters(params,
			{
				{ KeyID::VersionMajor, static_cast<int>(Version().Major()) },
				{ KeyID::VersionMinor, static_cast<int>(Version().Minor()) },
				{ KeyID::VersionBuild, static_cast<int>(Version().BuildNumber()) },
				{ KeyID::ClientPath, Path() }
			}, true));

		// Get the core init proc...
		pxCoreInitFunc pxcif = (pxCoreInitFunc)GetProcAddress(static_cast<HINSTANCE>(handle), pxCoreInitName.c_str());
		if (!pxcif)
			throw Exception(ErrorID::HandleError,
				{
					{ TagID::Filename, Str(pxCoreFilename) },
					{ TagID::Function, Str(pxCoreInitName) },
					{ TagID::Callee, Str(__FUNCTION__) }
				});

		// Call the core init function...
		void* instance = static_cast<CoreInterface*>(pxcif(msg,
			[](unsigned int err, const wchar_t* msg)
			{
				throw Exception(Str(std::to_wstring(err)), { { TagID::What, Str(msg) } });
			},
			InteropString{ static_cast<unsigned int>(paramsString.size()), paramsString.c_str() }));

		// Assign the idle function...
		coreIdleFunc = (pxCoreIdleFunc)GetProcAddress(static_cast<HINSTANCE>(handle), pxCoreIdleName.c_str());
		if (!coreIdleFunc)
			throw Exception(ErrorID::HandleError,
				{
					{ TagID::Filename, Str(pxCoreFilename) },
					{ TagID::Function, Str(pxCoreIdleName) },
					{ TagID::Callee, Str(__FUNCTION__) }
				});

		// Since this module itsef could have commands...
		Handshake(instance);
		
		// On this occasion output the version summary and welcome message...
		GetCoreInstance().Message(Name() + Str(" ") + Version().Summary());
	}

	Context::~Context()
	{
		// Free the library...
		if (handle)
			FreeLibrary(static_cast<HINSTANCE>(handle));
		handle = nullptr;
		coreInstance = nullptr;
	}

	void Context::Idle()
	{
		if (coreIdleFunc)
			coreIdleFunc();
	}

	// Kill the px instance
	void Context::Kill(const Parameters& params)
	{
		pxCoreKillFunc pxckf = (pxCoreKillFunc)GetProcAddress(static_cast<HINSTANCE>(handle), pxCoreKillName.c_str());
		if (pxckf)
		{
			// Since this is the handle (client module that owns the handle), drink the kool-aid for this module (handle owner)...
			Disconnect();

			// Call the core kill switch...
			auto paramsInteropString = To(params);
			pxckf({ static_cast<unsigned int>(paramsInteropString.size()), paramsInteropString.c_str() });

			// Free the library...
			if (handle)
				FreeLibrary(static_cast<HINSTANCE>(handle));
			
			handle = nullptr;
			coreInstance = nullptr;
		}
	}

	// Handle...
	String Context::Name()
	{
		if (pxModuleFilename.empty())
			AssignModuleFilepath();

		return Str(pxModuleFilename.stem().string());
	}

	String Context::Path()
	{
		if (pxModuleFilename.empty())
			AssignModuleFilepath();

		return Str(pxModuleFilename.string());
	}

	// Disconnect this handle from the core, prevents any more unregisters etc from this module calling the core
	void Context::Disconnect()
	{
		coreConnected = false;
	}

	void Context::Handshake(void* instance)
	{
		// Attempt to cast...
		coreInstance = static_cast<CoreInterface*>(instance);
		if (!coreInstance)
			throw Exception(ErrorID::HandshakeFail, { { TagID::Callee, Str(__FUNCTION__) } });

		// Check the version...
		auto coreVersion = coreInstance->Version();
		auto moduleVersion = Version();

		if (coreVersion.Major() != moduleVersion.Major())
		{
			throw Exception(ErrorID::VersionMismatch,
				{
					{ TagID::Expected, Str(std::to_string(moduleVersion.Major())) },
					{ TagID::What, Str(std::to_string(coreVersion.Major())) },
					{ TagID::Callee, Str(__FUNCTION__) }
				});
		}

		// The core is now connected :)
		coreConnected = true;

		// Syncornise the commands....
		RegisterCommands();

		// Output
		Message(Name() + Str(" ") + coreVersion.Summary());
	}

	void Context::Message(const String& msg)
	{
		GetCoreInstance().Message(msg);
	}

	void Context::ReportException(const std::vector<Exception>& e)
	{
		GetCoreInstance().ReportException(e);
	}

	// Mutex...
	Mutex::Mutex(const String& owner)
		: owner_(owner)
	{
#ifdef ENABLE_MUTEX_TRACE
		AxW::Message(AxW::Str("=== ") + owner_ + AxW::Str(" Locking ==="));
#endif // ENABLE_MUTEX_TRACE
		GetCoreInstance().GlobalLock(owner_);
	}

	Mutex::~Mutex()
	{
#ifdef ENABLE_MUTEX_TRACE
		AxW::Message(AxW::Str("=== ") + owner_ + AxW::Str(" Unlocking ==="));
#endif // ENABLE_MUTEX_TRACE
		GetCoreInstance().GlobalUnlock(owner_);
	}

	// CommandInterface...
	CommandInterface::CommandInterface(const String& name)
		: name_(name), owner_(Context::Name())
	{
		coreConnected ? GetCoreInstance().RegisterCommand(this) : RegisterCommand(this);
	}

	CommandInterface::CommandInterface(const String& name, Callback callback)
		: name_(name), owner_(Context::Name()), callback_(callback)
	{
		GetCoreInstance().RegisterCommand(this);
	}

	CommandInterface::~CommandInterface()
	{
		if (coreConnected)
			GetCoreInstance().UnregisterCommand(this);
	}

	bool Command(const String& syntax)
	{
		return GetCoreInstance().CommandInvoke(syntax);
	}

	bool Command(const std::string& syntax)
	{
		return GetCoreInstance().CommandInvoke(px::Str(syntax));
	}

	bool Command(const CommandInterface::Arguments& arguments)
	{
		return GetCoreInstance().CommandInvoke(arguments);
	}

	// ObserverInterface...
	ObserverInterface::ObserverInterface(const ID& id)
		: id_(id)
	{
		GetCoreInstance().RegisterObserver(this);
	}

	ObserverInterface::~ObserverInterface()
	{
		if (coreConnected)
			GetCoreInstance().UnregisterObserver(this);
	}

	void ObserverInterface::ToFront()
	{
		GetCoreInstance().ObserverToFront(this);
	}

	void ObserverInterface::ToBack()
	{
		GetCoreInstance().ObserverToBack(this);
	}

	ObserverList Observers(const ObserverInterface::ID& eventID)
	{
		return GetCoreInstance().Subscribers(eventID);
	}

	// Parameters...
	Parameter GetParameter(const Parameters& params, const Parameter::Key& parameterName, const Parameter& defaultParameterValue)
	{
		auto param = GetParameter(params, parameterName);
		return param ? *param : defaultParameterValue;
	}

	const Parameter* GetParameter(const Parameters& params, const Parameter::Key& parameterName)
	{
		auto itr = params.find(parameterName);
		return itr != params.end() ? &itr->second : nullptr;
	}

	Parameters From(const String& syntax)
	{
		return GetCoreInstance().From(syntax);
	}

	String To(const Parameters& parameters)
	{
		if (parameters.empty())
			return Str("");

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
				*stringVal = ReplaceAll(*stringVal, Str("\\"), Str("/"));
				oss << "\"" << *stringVal << "\"";
			}
			oss << ", ";
		}
		String result = Str(oss.str());
		result.pop_back();
		result.pop_back();
		result.append(Str(" }"));
		return result;
	}

	Parameters MergeParameters(const Parameters& params, const Parameters& paramsToMerge, bool overwrite)
	{
		// We create some parameters using the default parameters and merging the supplied parameters with them...
		Parameters result = params;

		// IF the default params contained the supplied param, replace the default with the supplied one, otherwise add it...
		if (overwrite)
		{
			for (auto param = paramsToMerge.begin(); param != paramsToMerge.end(); ++param)
				result[param->first] = param->second;
		}
		else
		{
			for (auto param = paramsToMerge.begin(); param != paramsToMerge.end(); ++param)
			{
				if (!GetParameter(result, param->first))
					result[param->first] = param->second;
			}
		}

		return result;
	}

	void Message(const String& msg)
	{
		GetCoreInstance().Message(msg);
	}

	void ReportException(const std::vector<Exception>& e)
	{
		GetCoreInstance().ReportException(e);
	}
}