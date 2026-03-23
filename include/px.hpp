#ifndef PX_HPP
#define PX_HPP

// A Generic platorm which provides easy module/plugin usage.
// A Command pattern
// An Event pattern
// A Python script invoker
// A Task interface (multi threading)
// A Process interface (child process communication)
// A GraphDB-esque datamodel with cypher query language support

// Designed to be extendable, so users can derive the interfaces they are interested in and let pX platform manager them...
// pxg PX graphics engine is a draw engine to be used
// 

#include "px/Type.hpp"
#include "px/Exception.hpp"
#include "px/Mutex.hpp"
#include "px/Command.hpp"
#include "px/Draw.hpp"
#include "px/Context.hpp"
#include "px/Event.hpp"
#include "px/Task.hpp"
#include "px/Data.hpp"
//#include "px/Util.hpp"
#include "px/Display.hpp"
#include "px/Geometry.hpp"

#include <functional>


#ifndef PX_WIN
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PX_WIN
#endif
#endif

#ifndef PX_NIX
#if defined(__linux__) || defined(__unix__)
#define PX_NIX
#endif
#endif

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
PX_ERROR(NotKilled)
PX_ERROR(TaskInitFail)
PX_ERROR(EntityAlreadyExists)
PX_ERROR(EntityNotFound)
PX_ERROR(DataModelAlreadyExists)
PX_ERROR(DataModelNotFound)
PX_ERROR(DisplayError)
PX_ERROR(DrawObjectNotFound)
PX_ERROR(DrawObjectAlreadyRegistered)
PX_ERROR(TextureNotFound)
PX_ERROR(TextureAlreadyRegistered)
PX_ERROR(ViewNotFound)
PX_ERROR(ViewAlreadyRegistered)
PX_ERROR(NYI)

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
PX_TAG(Result)
PX_TAG(Project)
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
PX_EVENT_1(TaskStart, unsigned int, threadID)
PX_EVENT_1(TaskFinish, unsigned int, threadID)
PX_EVENT_2(EntityAdded, const String&, projectName, const String&, entityName)
PX_EVENT_2(EntityRemoved, const String&, projectName, const String&, entityName)
PX_EVENT_2(EntityChanged, const String&, projectName, const String&, entityName)
PX_EVENT_2(EntityTopology, const String&, projectName, const String&, entityName)
PX_EVENT_1(DataModelAdded, const String&, projectName)
PX_EVENT_1(DataModelRemoved, const String&, projectName)
PX_EVENT_1(DataModelChanged, const String&, projectName)
PX_EVENT_2(DataModelRenamed, const String&, newName, const String&, oldName)

// Draw events..
PX_EVENT_1(DrawObjectGeometryChanged, const px::DrawObject&, drawObject)
PX_EVENT_1(DrawObjectTransformChanged, const px::DrawObject&, drawObject)
PX_EVENT_1(DrawObjectRegistered, const px::DrawObject&, drawObject)
PX_EVENT_1(DrawObjectUnregistered, const px::DrawObject&, drawObject)
PX_EVENT_1(DrawObjectMaterialChanged, const px::DrawObject&, drawObject)
PX_EVENT_2(DisplayResize, unsigned int, width, unsigned int, height)
PX_EVENT_1(ViewRegistered, const String&, viewName)
PX_EVENT_1(ViewUnregistered, const String&, viewName)
PX_EVENT_1(ViewShow, const View&, view)
PX_EVENT_1(ViewHide, const View&, view)
PX_EVENT_1(ViewResize, const View&, view)
PX_EVENT_1(TextureRegistered, const px::Texture&, texture)
PX_EVENT_1(TextureUnregistered, const px::Texture&, texture)
PX_EVENT_1(TextureChanged, const px::Texture&, texture)

// Client events...
PX_EVENT(ClientStartIdle)
PX_EVENT(ClientEndIdle)
PX_EVENT_1(ClientRedraw, int, displayID)
PX_EVENT_3(ClientOpenFile, const String&, title, const String&, path, const std::vector<String>&, validExtensions)
PX_EVENT_1(ClientOpenFileResult, const String&, filepath)
PX_EVENT_3(ClientMultiOpenFile, const String&, title, const String&, path, const std::vector<String>&, validExtensions)
PX_EVENT_1(ClientMultiOpenFileResult, const std::vector<String>&, filepaths)
PX_EVENT_3(ClientSaveFile, const String&, title, const String&, path, const std::vector<String>&, validExtensions)
PX_EVENT_1(ClientSaveFileResult, const String&, filepath)
PX_EVENT_2(ClientChooseFolder, const String&, title, const String&, path)
PX_EVENT_1(ClientChooseFolderResult, const String&, filepath)
PX_EVENT_3(ClientQuestion, const String&, title, const String&, question, const std::vector<String>&, answers)
PX_EVENT_1(ClientQuestionResult, const String&, answer)
PX_EVENT_2(ClientMessageBox, const String&, title, const String&, message)
PX_EVENT_1(ClientMousePointer, int, pointer)
PX_EVENT_1(ClientDropFiles, const std::vector<String>&, droppedFiles)
PX_EVENT(ClientStartAnimation)
PX_EVENT(ClientEndAnimation)


namespace px
{
	void Message(const px::String& msg);
	void ReportException(const std::vector<px::Exception>& e);
}



#endif // PX_HPP