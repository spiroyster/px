#include "GLInstance.hpp"
#include "GLContext.hpp"

namespace pxgl
{
	GLInstance::GLInstance(const std::shared_ptr<GLContext>& context)
		: contexts_({ context })
	{
	}

	void GLInstance::Free()
	{

	}

	void GLInstance::DrawObjectRegisteredEvent(const px::DrawObject& drawObject)
	{

	}

	void GLInstance::DrawObjectUnregisteredEvent(const px::DrawObject& drawObject)
	{

	}

	void GLInstance::DrawObjectGeometryChangedEvent(const px::DrawObject& drawObject)
	{

	}

	void GLInstance::DrawObjectMaterialChangedEvent(const px::DrawObject& drawObject)
	{

	}

	void GLInstance::DrawObjectTransformChangedEvent(const px::DrawObject& drawObject)
	{

	}

	void GLInstance::TextureRegisteredEvent(const px::Texture& texture)
	{

	}

	void GLInstance::TextureUnregisteredEvent(const px::Texture& texture)
	{

	}

	void GLInstance::TextureChangedEvent(const px::Texture& texture)
	{

	}

	void GLInstance::HouseKeeping(const GLContext& context)
	{

	}

	std::shared_ptr<GLDrawObject> GLInstance::FindDrawObject(unsigned int pickerID)
	{
		return std::shared_ptr<GLDrawObject>();
	}

	std::shared_ptr<GLTexture> GLInstance::FindTexture(const px::String& textureName)
	{
		return std::shared_ptr<GLTexture>();

	}

	std::shared_ptr<GLInstance::CurrentContextMutex> GLInstance::GetContext(void* dc)
	{
		// Prevent any other context making current...
		//std::lock_guard<std::mutex> guard(context_mutex_);

		// N.B This should only be created for compute
		auto threadID = std::this_thread::get_id();
		auto context = std::find_if(contexts_.begin(), contexts_.end(), [&threadID](std::shared_ptr<GLContext>& c)
			{
				return c->GetThreadID() == threadID;
			});

		if (context != contexts_.end())
			return std::make_shared<CurrentContextMutex>(**context);

		// Otherwise we create the context...
		contexts_.emplace_back(std::make_shared<GLContext>(threadID, dc, nullptr, contexts_.empty() ? nullptr : &(*contexts_.front())));

		return std::make_shared<CurrentContextMutex>(*contexts_.back());
	}

	std::shared_ptr<GLInstance::CurrentContextMutex> GLInstance::GetMainThreadContext()
	{
		if (contexts_.empty())
			throw px::Exception(px::ErrorID::DisplayError,
				{
					{ px::TagID::What, px::Str("Unable to get main thread context.") },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		return std::make_shared<CurrentContextMutex>(*contexts_.front());
	}

	GLInstance::CurrentContextMutex::CurrentContextMutex(GLContext& context)
		: lock_(GetGLInstance()->context_mutex_), context_(context)
	{
		context_.MakeThisContextCurrent();
		/*if (GetGLInstance()->debugTracing_)
		{
			std::wstringstream wss;
			wss << "CurrentContextMutex " << static_cast<unsigned int>(std::hash<std::thread::id>{}(context.GetThreadID()));
			OutputDebugString(wss.str().c_str());
		}*/
	}

	GLInstance::CurrentContextMutex::~CurrentContextMutex()
	{
		wglMakeCurrent(NULL, NULL);
		if (GetGLInstance()->debugTracing_)
			OutputDebugString(L"CurrentContextMutex destructed");
	}
}