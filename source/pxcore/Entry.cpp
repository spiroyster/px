#include "instance.hpp"

#include <include/px.hpp>

#define RAPIDJSON_HAS_STDSTRING 1
#include <thirdparty/rapidjson/include/rapidjson/document.h>
#include <thirdparty/rapidjson/include/rapidjson/prettywriter.h>
#include <thirdparty/rapidjson/include/rapidjson/stringbuffer.h>

#include <memory>

#ifndef PX_CORE_FUNC
#define PX_CORE_FUNC extern "C" __declspec(dllexport) 
#endif

namespace pxCore
{
	namespace
	{
		std::unique_ptr<Instance> instance_;

		px::Parameters From(const px::String& syntax)
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
	}

	Instance& GetInstance()
	{
		if (instance_)
			return *instance_;

		throw px::Exception(px::ErrorID::InstanceInvalid,
			{
				{ px::TagID::Callee, px::Str(__FUNCTION__) }
			});
	}
}

/// <summary>
/// </summary>
/// <param name="msgCallback">Callback for messages.</param>
/// <param name="exceptionCallback">Callback for exceptions</param>
/// <param name="configurationPath">Path to configuration file to use.</param>
/// <param name="appPath">Path to application folder.</param>
/// <param name="userDocumentsPath">Paht to the users documents folder.</param>
/// <returns>Raw pointer to instantiated AxW Core instance.</returns>
PX_CORE_FUNC void* pxCoreInit(
	InitialiseMessageCallback msgCallback,
	InitialiseExceptionCallback exceptionCallback,
	InteropString parametersInterop)
{
	try
	{
		if (pxCore::instance_)
			throw px::Exception(px::ErrorID::InitialiseFail,
				{
					{ px::TagID::What, px::Str("Already initialised") }
				});

		// Rebuild the parameters from the interop string...
		px::Parameters parameters = pxCore::From(parametersInterop.ToStdString());

		// First check the version and ensure that major versions match...
		if (auto param = px::GetParameter(parameters, px::String(px::KeyID::VersionMajor)))
		{

			// Set the application path (N.B this is read only)
			if (auto ver = param->GetInt())
			{
				if (*ver != px::Context::Version().Major())
					throw px::Exception(px::ErrorID::VersionMismatch,
						{
							{ px::TagID::Expected, px::Str(std::to_string(px::Context::Version().Major()))},
							{ px::TagID::Value, px::Str(std::to_string(*ver)) },
							{ px::TagID::Callee, px::Str(__FUNCTION__) }
						});
			}
			else
				throw px::Exception(px::ErrorID::ParameterValueInvalid,
					{
						{ px::TagID::Key, px::String(px::KeyID::VersionMajor) },
						{ px::TagID::Expected, px::Str("Integer") },
						{ px::TagID::Callee, px::Str(__FUNCTION__) }
					});
		}
		else
			throw px::Exception(px::ErrorID::ParameterKeyInvalid,
				{
					{ px::TagID::Key, px::String(px::KeyID::VersionMajor) },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		// Instantiate the AxW Core...
		pxCore::instance_ = std::make_unique<pxCore::Instance>();

		// Set the parameters...
		pxCore::instance_->GetParameters() = parameters;
		
		// This core is a module, so handshake it, with itself...
		px::Context::Handshake(static_cast<void*>(pxCore::instance_.get()));
		
		// Create a callback to pipe messages to the initialise callback (from this point on px::Message can be used)...
		auto pipeMessageCallback = px::Observer::Message([=](const px::String& msg) { msgCallback(msg.c_str()); });

		// If there is a log file specified, use it...
		if (auto logFileParam = px::GetParameter(pxCore::instance_->GetParameters(), px::KeyID::LogFile))
		{

		}

		// Display the welcome message...
		px::Context::Message(px::Context::Name() + px::Str(" ") + px::Context::Version().Summary());

		// Initialise the core instance. This performs any other instance secondary intialisation after the system has been setup...
		pxCore::instance_->Initialise();

		// Assign default parameters (namely the modules path)...
		auto modulePathParam = px::GetParameter(pxCore::instance_->GetParameters(), px::KeyID::ModulesPath);
		auto clientPathParam = px::GetParameter(pxCore::instance_->GetParameters(), px::KeyID::ClientPath);
		if (!modulePathParam)
		{
			std::filesystem::path clientPath(*clientPathParam->GetString());
			pxCore::instance_->GetParameters()[px::KeyID::ModulesPath] = px::Str(clientPath.parent_path().string());
		}

		// Finished (return the coreInstance to the handle owner (a client) to handshake with the client...
		return static_cast<void*>(pxCore::instance_.get());
	}
	catch (const px::Exception& e)
	{
		exceptionCallback(2, px::To(e.Meta()).c_str());
	}
	catch (const std::exception& e)
	{
		exceptionCallback(1, px::Str(e.what()).c_str());
	}
	catch (...)
	{
		// Swallow all exceptions, cannot let escape across the fence (execution boundary)...
		exceptionCallback(0, px::Str("").c_str());
	}

	return nullptr;
}


/// <summary>
/// Terminate the px context.
/// </summary>
PX_CORE_FUNC void pxCoreKill(InteropString killParam)
{
	// We call terminate 
	pxCore::instance_->Terminate();

	// Disconnect from core, prevent events/unregisters from using manager
	px::Context::Disconnect();

	// Free (this will destroy the managers)...
	pxCore::instance_.reset();
}

PX_CORE_FUNC void pxCoreIdle()
{
	//px::System::Mutex mut(px::Str("AxwCoreIdle"));

	// Process the tasks (N.B this is also done in Render)...
	//AxWCore::GetInstance().ProcessTasks();

	// Process the animations...
	//AxWCore::GetInstance().ProcessAnimations();

	// Process the scheduled compute shaders...
	//if (px::System::Display::AdaptorHandle* displayAdaptor = AxWCore::GetInstance().DisplayAdaptor())
	//	displayAdaptor->Idle();

}