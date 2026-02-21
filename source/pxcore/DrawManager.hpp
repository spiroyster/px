#ifndef PXCORE_DRAWMANAGER_HPP
#define PXCORE_DRAWMANAGER_HPP

#include <include/px/Type.hpp>
#include <map>

namespace px
{
	class View;
	class DrawObject;
	class Texture;
}

namespace pxCore
{
	class DrawManager
	{
	public:
		void RegisterView(px::View* view);
		void UnregisterView(px::View* view);
		const std::map<px::String, px::View*>& Views() const;
		void RegisterDrawObject(px::DrawObject* sceneObject);
		void UnregisterDrawObject(px::DrawObject* sceneObject);
		const std::map<px::String, px::DrawObject*>& DrawObjects() const;
		void RegisterTexture(px::Texture* texture);
		void UnregisterTexture(px::Texture* texture);
		const std::map<px::String, px::Texture*>& Textures() const;
		
	private:
		std::map<px::String, px::View*> views_;
		std::map<px::String, px::DrawObject*> drawObjects_;
		std::map<px::String, px::Texture*> textures_;
		
		//std::mutex mutex_;
	};
}

#endif // PXCORE_DRAWMANAGER_HPP