#ifndef PX_CONTEXT_HPP
#define PX_CONTEXT_HPP

#include <functional>

#include "Interop.hpp"
#include "Version.hpp"
#include "Parameter.hpp"
#include "Exception.hpp"

namespace px
{
	// client creates the handle, and then passes OS events...
	// idle
	// kill
	// command
	// event

	class Context
	{

	public:

		//typedef std::function<void(const String& msg)> MessageCallback;
		//typedef std::function<void(const Exception& e)> ExceptionCallback;

		Context(const Parameters& params, InitialiseMessageCallback msg);

		~Context();

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

// N.B Client should not use AXW_MAIN or AXW_KILL!!!
#define PX_MAIN \
void pxMainImpl();\
extern "C" __declspec(dllexport) bool pxModuleMain(void* instance)\
{\
	try \
	{\
		px::Context::Handshake(instance);\
		pxMainImpl();\
		return true;\
	}\
	catch (const px::Exception& e)\
	{\
		px::Context::ReportException({e});\
	}\
	catch (const std::exception& e)\
	{\
		px::Context::ReportException({px::Exception(e)});\
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

#endif // PX_CONTEXT_HPP