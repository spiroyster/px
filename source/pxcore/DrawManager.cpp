#include "DrawManager.hpp"
#include <include/px.hpp>

namespace pxCore
{
	void DrawManager::RegisterView(px::View* view)
	{
		auto itr = views_.find(view->ViewName());
		if (itr != views_.end())
			throw px::Exception(px::ErrorID::ViewAlreadyRegistered,
				{
					{ px::TagID::Name, view->ViewName() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		views_[view->ViewName()] = view;
		px::Dispatch::ViewRegistered(view->ViewName());
	}

	void DrawManager::UnregisterView(px::View* view)
	{
		auto itr = views_.find(view->ViewName());
		if (itr == views_.end())
			throw px::Exception(px::ErrorID::ViewNotFound,
				{
					{ px::TagID::Name, view->ViewName() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		views_.erase(itr);
		px::Dispatch::ViewUnregistered(view->ViewName());
	}

	const std::map<px::String, px::View*>& DrawManager::Views() const
	{
		return views_;
	}

	void DrawManager::RegisterDrawObject(px::DrawObject* drawObject)
	{
		// Check if this scene is already added, in which case do nothing...
		auto itr = drawObjects_.find(drawObject->DrawObjectName());
		if (itr != drawObjects_.end())
			throw px::Exception(px::ErrorID::DrawObjectAlreadyRegistered,
				{
					{ px::TagID::Name, drawObject->DrawObjectName() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		drawObjects_[drawObject->DrawObjectName()] = drawObject;
		px::Dispatch::DrawObjectRegistered(drawObject->DrawObjectName());
	}

	void DrawManager::UnregisterDrawObject(px::DrawObject* drawObject)
	{
		auto itr = drawObjects_.find(drawObject->DrawObjectName());
		if (itr == drawObjects_.end())
			throw px::Exception(px::ErrorID::DrawObjectNotFound,
				{
					{ px::TagID::Name, drawObject->DrawObjectName() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		// Tell the world before we remove the object..
		px::Dispatch::DrawObjectUnregistered(drawObject->DrawObjectName());

		drawObjects_.erase(itr);
	}

	const std::map<px::String, px::DrawObject*>& DrawManager::DrawObjects() const
	{
		return drawObjects_;
	}

	void DrawManager::RegisterTexture(px::Texture* texture)
	{
		// Check if this scene is already added, in which case do nothing...
		auto itr = textures_.find(texture->TextureName());
		if (itr != textures_.end())
			throw px::Exception(px::ErrorID::TextureAlreadyRegistered,
				{
					{ px::TagID::Name, texture->TextureName() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		textures_[texture->TextureName()] = texture;
		px::Dispatch::TextureRegistered(*texture);
	}

	void DrawManager::UnregisterTexture(px::Texture* texture)
	{
		auto itr = textures_.find(texture->TextureName());
		if (itr == textures_.end())
			throw px::Exception(px::ErrorID::TextureNotFound,
				{
					{ px::TagID::Name, texture->TextureName() },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		textures_.erase(itr);
		px::Dispatch::TextureUnregistered(*texture);
	}

	const std::map<px::String, px::Texture*>& DrawManager::Textures() const
	{
		return textures_;
	}
}