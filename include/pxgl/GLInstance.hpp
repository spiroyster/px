#ifndef PXGL_INSTANCE_HPP
#define PXGL_INSTANCE_HPP

#include "../px/include/px.hpp"
#include "../px/include/px/Display.hpp"

#include <mutex>
#include <map>

namespace pxgl
{
	class GLDrawObject;
	class GLTexture;
	class GLContext;
	
	class GLInstance :
		public px::Display,
		public px::Observer::DrawObjectRegistered,
		public px::Observer::DrawObjectUnregistered,
		public px::Observer::DrawObjectGeometryChanged,
		public px::Observer::DrawObjectMaterialChanged,
		public px::Observer::DrawObjectTransformChanged,
		public px::Observer::TextureRegistered,
		public px::Observer::TextureUnregistered,
		public px::Observer::TextureChanged,
		public px::Observer::TaskFinish
	{
	public:
		GLInstance(void* dc, void* hglrc);
		GLInstance(void* dc);

		void Free();

		// px::Display...
		void Render(void* dc);
		void Resize(void* dc, unsigned int width, unsigned int height);

		px::Display::Coordinate GetDisplayCoordinate(const px::Display::CoordinateNormalised & dc);
		px::Display::CoordinateNormalised GetDisplayCoordinateNormalised(const px::Display::Coordinate& dc);
		std::pair<px::View*, px::View::Coordinate> GetViewCoordinate(const px::Display::Coordinate& dc);
		std::optional<px::Display::Coordinate> GetDisplayCoordinate(const px::View& view, const px::View::Coordinate& vc);
		std::shared_ptr<px::ImageRGBA32> FrameImage();
		const px::DrawObject* Interrogate(const Display::Coordinate& xy) const;
		px::Parameters GetContextInfo() const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		// px::Events...
		void DrawObjectRegisteredEvent(const px::DrawObject& drawObject);
		void DrawObjectUnregisteredEvent(const px::DrawObject& drawObject);
		void DrawObjectGeometryChangedEvent(const px::DrawObject& drawObject);
		void DrawObjectMaterialChangedEvent(const px::DrawObject& drawObject);
		void DrawObjectTransformChangedEvent(const px::DrawObject& drawObject);
		void TextureRegisteredEvent(const px::Texture& texture);
		void TextureUnregisteredEvent(const px::Texture& texture);
		void TextureChangedEvent(const px::Texture& texture);
		void TaskFinishEvent(unsigned int threadID);

		// GL
		void HouseKeeping(const GLContext& context);

		std::shared_ptr<GLDrawObject> FindDrawObject(unsigned int pickerID);
		std::shared_ptr<GLTexture> FindTexture(const px::String& textureName);

		// Context manager...
		class CurrentContextMutex
		{
			std::lock_guard<std::mutex> lock_;
			GLContext& context_;
		public:
			CurrentContextMutex(GLContext& context);
			GLContext& GetContext() const { return context_; }
			~CurrentContextMutex();
		};

		std::shared_ptr<CurrentContextMutex> GetContext(void* dc);
		std::shared_ptr<CurrentContextMutex> GetMainThreadContext();
		
		
		bool DebugTracing() { return debugTracing_; }
		void DebugTracing(bool trace) { debugTracing_ = trace; }

	private:
		// Our resource managers...
		std::list<std::shared_ptr<GLDrawObject>> drawObjects_;
		std::map<px::String, std::shared_ptr<GLTexture>> textures_;
		std::list<std::shared_ptr<GLContext>> contexts_;

		// Display width, height...
		unsigned int width_;
		unsigned int height_;
		
		// Picker counter...
		unsigned int pickerCounter_ = 100;

		bool textureRecacheRequired_ = false;
		bool cleanupDestroyedRequired_ = false;

		bool debugTracing_ = false;
		int scheduledProcessPollRate_ = 200;

		// current context lock...
		std::mutex context_mutex_;									// Race condition for adding alinger event...

		//std::shared_ptr<GLComputeInstance> glslCompute_;
	};

	extern GLInstance* GetGLInstance();

}


#endif // AxWGL_INSTANCE_HPP
