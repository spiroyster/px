#ifndef PX_CLIENT_HPP
#define PX_CLIENT_HPP

#include <functional>

#include "interop.hpp"
#include "version.hpp"
#include "parameter.hpp"
#include "exception.hpp"

namespace px
{
	// client creates the handle, and then passes OS events...
	// idle
	// kill
	// command
	// event

	class Handle
	{

	public:

		//typedef std::function<void(const String& msg)> MessageCallback;
		//typedef std::function<void(const Exception& e)> ExceptionCallback;

		Handle(const Parameters& params, InitialiseMessageCallback msg);

		~Handle();

		void Idle();
		void Kill(const Parameters& params = {});
		
		static String Name();
		static String Path();
		static void Handshake(void* instance);
		static void Disconnect();

		static void Message(const String& msg);
		static void ReportException(const std::vector<Exception>& e);

		static VersionInformation Version();
	};

}

//#define PX_KEY(key) \
//namespace px { namespace ErrorID { static const Exception::ID err = px::String(#err); } }


// N.B Client should not use AXW_MAIN or AXW_KILL!!!
#define PX_MAIN \
void pxMainImpl();\
extern "C" __declspec(dllexport) bool pxModuleMain(void* instance)\
{\
	try \
	{\
		px::Handle::Handshake(instance);\
		pxMainImpl();\
		return true;\
	}\
	catch (const px::Exception& e)\
	{\
		px::Handle::ReportException({e});\
	}\
	catch (const std::exception& e)\
	{\
		px::Handle::ReportException({px::Exception(e)});\
	}\
	catch (...)\
	{\
	}\
	return false;\
}\
void pxMainImpl()

#define PX_KILL \
void pxKillImpl();\
extern "C" __declspec(dllexport) void pxModuleKill()\
{\
	try\
	{\
		pxKillImpl();\
	}\
	catch (...)\
	{\
	}\
}\
void pxKillImpl()

#endif // PX_CLIENT_HPP