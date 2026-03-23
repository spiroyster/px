#include "GLInstance.hpp"
#include "GLContext.hpp"
#include "GLTexture.hpp"
#include "GLDrawObject.hpp"

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
		auto itr = std::find_if(drawObjects_.begin(), drawObjects_.end(), 
			[&drawObject](const std::shared_ptr<GLDrawObject>& glDrawObject) 
			{ 
				return glDrawObject->drawObject_->DrawObjectName() == drawObject.DrawObjectName();
			});

		if (itr == drawObjects_.end())
			drawObjects_.emplace_back(std::make_shared<GLDrawObject>(&drawObject, pickerCounter_++));
		else
			*itr = std::make_shared<GLDrawObject>(&drawObject, (*itr)->PickerID());
	}

	void GLInstance::DrawObjectUnregisteredEvent(const px::DrawObject& drawObject)
	{
		auto itr = std::find_if(drawObjects_.begin(), drawObjects_.end(),
			[&drawObject](const std::shared_ptr<GLDrawObject>& glDrawObject)
			{
				return glDrawObject->drawObject_->DrawObjectName() == drawObject.DrawObjectName();
			});

		if (itr != drawObjects_.end())
		{
			(*itr)->SetDirtyState(GLDrawObject::Destroyed);
			cleanupDestroyedRequired_ = true;
		}
	}

	void GLInstance::DrawObjectGeometryChangedEvent(const px::DrawObject& drawObject)
	{
		auto itr = std::find_if(drawObjects_.begin(), drawObjects_.end(),
			[&drawObject](const std::shared_ptr<GLDrawObject>& glDrawObject)
			{
				return glDrawObject->drawObject_->DrawObjectName() == drawObject.DrawObjectName();
			});

		if (itr != drawObjects_.end())
			(*itr)->SetDirtyState(GLDrawObject::GeometryDirty);
	}

	void GLInstance::DrawObjectMaterialChangedEvent(const px::DrawObject& drawObject)
	{
		auto itr = std::find_if(drawObjects_.begin(), drawObjects_.end(),
			[&drawObject](const std::shared_ptr<GLDrawObject>& glDrawObject)
			{
				return glDrawObject->drawObject_->DrawObjectName() == drawObject.DrawObjectName();
			});

		if (itr != drawObjects_.end())
			(*itr)->SetDirtyState(GLDrawObject::MaterialDirty);
	}

	void GLInstance::DrawObjectTransformChangedEvent(const px::DrawObject& drawObject)
	{
		auto itr = std::find_if(drawObjects_.begin(), drawObjects_.end(),
			[&drawObject](const std::shared_ptr<GLDrawObject>& glDrawObject)
			{
				return glDrawObject->drawObject_->DrawObjectName() == drawObject.DrawObjectName();
			});

		if (itr != drawObjects_.end())
			(*itr)->SetDirtyState(GLDrawObject::TransformDirty);
	}

	void GLInstance::TextureRegisteredEvent(const px::Texture& texture)
	{
		auto itr = textures_.find(texture.TextureName());
		if (itr == textures_.end())
			textures_[texture.TextureName()] = std::make_shared<GLTexture>(&texture);

		// If we already have this texture, that implies that maybe it was destroyed before, and hasn't been cleaned up yet, so simply ...
		else
			itr->second->SetDirtyState(texture.GetImageShared());

		textureRecacheRequired_ = true;
	}

	void GLInstance::TextureUnregisteredEvent(const px::Texture& texture)
	{
		auto itr = textures_.find(texture.TextureName());
		
		// Flag the texutre as destroyed...
		if (itr != textures_.end())
		{
			itr->second->SetDirtyState(GLTexture::Destroyed);
			cleanupDestroyedRequired_ = true;
		}
	}

	void GLInstance::TextureChangedEvent(const px::Texture& texture)
	{
		auto itr = textures_.find(texture.TextureName());

		// Flag the texutre for recahcing...
		if (itr != textures_.end())
		{
			itr->second->SetDirtyState(texture.GetImageShared());
			textureRecacheRequired_ = true;
		}
	}

	void GLInstance::HouseKeeping(const GLContext& context)
	{
		// First remove all the items that require removing...
		if (cleanupDestroyedRequired_)
		{
			// Geometry...
			for (auto glDrawObject = drawObjects_.begin(); glDrawObject != drawObjects_.end();)
			{
				if ((*glDrawObject)->GetDirtyState() == GLDrawObject::Destroyed)
				{
					(*glDrawObject)->Free(context);
					glDrawObject = drawObjects_.erase(glDrawObject);
				}
				else
					++glDrawObject;
			}

			// Textures...
			for (auto glTextureItr = textures_.begin(); glTextureItr != textures_.end();)
			{
				if (glTextureItr->second->GetDirtyState() == GLTexture::Destroyed)
				{
					glTextureItr->second->Free();
					glTextureItr = textures_.erase(glTextureItr);
				}
				else
					++glTextureItr;
			}

			cleanupDestroyedRequired_ = false;
		}

		// For now just update the textures (geometry done as part of scene list refresh)...
		if (textureRecacheRequired_)
		{
			for (auto glTextureItr = textures_.begin(); glTextureItr != textures_.end(); ++glTextureItr)
				glTextureItr->second->Cache();

			textureRecacheRequired_ = false;
		}

	}

	std::shared_ptr<GLDrawObject> GLInstance::FindDrawObject(unsigned int pickerID)
	{
		for (auto itr = drawObjects_.begin(); itr != drawObjects_.end(); ++itr)
			if ((*itr)->PickerID() == pickerID)
				return *itr;
		return std::shared_ptr<GLDrawObject>();
	}

	std::shared_ptr<GLTexture> GLInstance::FindTexture(const px::String& textureName)
	{
		auto itr = textures_.find(textureName);
		return itr == textures_.end() ? std::shared_ptr<GLTexture>() : itr->second;
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
		context_.ContextMakeCurrent();
		/*if (GetGLInstance()->debugTracing_)
		{
			std::wstringstream wss;
			wss << "CurrentContextMutex " << static_cast<unsigned int>(std::hash<std::thread::id>{}(context.GetThreadID()));
			OutputDebugString(wss.str().c_str());
		}*/
	}

	GLInstance::CurrentContextMutex::~CurrentContextMutex()
	{
		context_.ContextDisable();
		//if (GetGLInstance()->debugTracing_)
		//	OutputDebugString(L"CurrentContextMutex destructed");
	}
}