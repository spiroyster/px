#ifndef PX_HPP
#define PX_HPP

// A Generic platorm which provides easy module/plugin usage.
// A Command pattern
// An Event pattern
// A Python
// A Task interface (multi threading)
// A Process interface (child process communication)
// A GraphDB-esque datamodel with cypher query language support

// Designed to be extendable, so users can derive the interfaces they are interested in and let pX platform manager them...
// pxg PX graphics engine is a draw engine to be used
// 

#include "type.hpp"
#include "exception.hpp"
#include "mutex.hpp"
#include "command.hpp"
#include "handle.hpp"
#include "event.hpp"
#include "data.hpp"

#include <functional>

// pX errors...
PX_ERROR(InstanceInvalid)
PX_ERROR(HandleError)
PX_ERROR(HandshakeFail)
PX_ERROR(VersionMismatch)
PX_ERROR(UnknownError)
PX_ERROR(StdException)
PX_ERROR(InitialiseFail)
PX_ERROR(ParameterKeyInvalid)
PX_ERROR(ParameterValueInvalid)
PX_ERROR(ParametersSyntax)
PX_ERROR(CommandAlreadyRegistered)
PX_ERROR(CommandNotFound)
PX_ERROR(CommandSyntaxInvalid)
PX_ERROR(ObserverInvalid)
PX_ERROR(ObserverNotFound)
PX_ERROR(ModuleAlreadyRegistered)
PX_ERROR(ModuleNotFound)
PX_ERROR(ModuleLoadFail)
PX_ERROR(FileNotFound)
PX_ERROR(FileRead)
PX_ERROR(FileLineInvalid)

// px tags...
PX_TAG(Filename)
PX_TAG(Function)
PX_TAG(Callee)
PX_TAG(Expected)
PX_TAG(What)
PX_TAG(Key)
PX_TAG(Value)
PX_TAG(LineNumber)
PX_TAG(Name)
PX_TAG(ID)
PX_TAG(Thread)
PX_TAG(Offender)
PX_TAG(Error)
PX_TAG(Command)
PX_TAG(Owner)
PX_TAG(MacroLineNumber)
PX_TAG(MacroLineSyntax)

// px keys
PX_KEY(VersionMajor)
PX_KEY(VersionMinor)
PX_KEY(VersionBuild)
PX_KEY(ClientPath)			// The client path to look for modules
PX_KEY(ModulesPath)			// The modules path to look for modules failing client
PX_KEY(MacroHaltOnError)
PX_KEY(LogFile)				// If defined, specifies log filename and path

// pX commands...

// pX events...
PX_EVENT_1(Message, const String&, msg)
PX_EVENT_1(Exception, const std::vector<px::Exception>&, e)
PX_EVENT(Kill)
PX_EVENT_1(CommandInvoke, const px::CommandInterface::Arguments&, arguments)
PX_EVENT_1(CommandFail, const px::CommandInterface::Arguments&, arguments)
PX_EVENT_1(CommandSucceed, const px::CommandInterface::Arguments&, arguments)
PX_EVENT_1(ModuleInstall, const px::String&, name)
PX_EVENT_1(ModuleUninstall, const px::String&, name)
PX_EVENT_1(EntityAdd, const std::shared_ptr<px::Entity>&, entity)
PX_EVENT_1(EntityRemove, const std::shared_ptr<px::Entity>&, entity)
PX_EVENT_1(EntityTopology, const std::shared_ptr<px::Entity>&, entity)
PX_EVENT_1(EntityModify, const std::shared_ptr<px::Entity>&, entity)

//// Task events...
//AxW_EVENT_1(TaskStart, unsigned int, threadID)
//AxW_EVENT_1(TaskFinish, unsigned int, threadID)
//
//// Client events...
//AxW_EVENT(ClientStartIdle)
//AxW_EVENT(ClientEndIdle)
//AxW_EVENT_1(ClientRedraw, int, displayID)
//AxW_EVENT_3(ClientOpenFile, const String&, title, const String&, path, const std::vector<String>&, validExtensions)
//AxW_EVENT_1(ClientOpenFileResult, const String&, filepath)
//AxW_EVENT_3(ClientMultiOpenFile, const String&, title, const String&, path, const std::vector<String>&, validExtensions)
//AxW_EVENT_1(ClientMultiOpenFileResult, const std::vector<String>&, filepaths)
//AxW_EVENT_3(ClientSaveFile, const String&, title, const String&, path, const std::vector<String>&, validExtensions)
//AxW_EVENT_1(ClientSaveFileResult, const String&, filepath)
//AxW_EVENT_2(ClientChooseFolder, const String&, title, const String&, path)
//AxW_EVENT_1(ClientChooseFolderResult, const String&, filepath)
//AxW_EVENT_3(ClientQuestion, const String&, title, const String&, question, const std::vector<String>&, answers)
//AxW_EVENT_1(ClientQuestionResult, const String&, answer)
//AxW_EVENT_2(ClientMessageBox, const String&, title, const String&, message)
//AxW_EVENT_1(ClientMousePointer, int, pointer)
//AxW_EVENT_1(ClientDropFiles, const std::vector<String>&, droppedFiles)
//AxW_EVENT(ClientStartAnimation)
//AxW_EVENT(ClientEndAnimation)


namespace px
{
	void Message(const px::String& msg);
	void ReportException(const std::vector<px::Exception>& e);
}



#endif // PX_HPP