#ifndef PXGL_INSTANCE_HPP
#define PXGL_INSTANCE_HPP

#include <include/px.hpp>
#include <include/px/Display.hpp>

#include <mutex>
#include <map>

namespace pxgl
{
	class GLDrawObject;
	class GLTexture; 
	class GLContext;
	//class GLShaderManager;
	//class GLComputeContext;

	class GLInstance :
		public px::Observer::DrawObjectRegistered,
		public px::Observer::DrawObjectUnregistered,
		public px::Observer::DrawObjectGeometryChanged,
		public px::Observer::DrawObjectMaterialChanged,
		public px::Observer::DrawObjectTransformChanged,
		public px::Observer::TextureRegistered,
		public px::Observer::TextureUnregistered,
		public px::Observer::TextureChanged
	{
	public:
		GLInstance(const std::shared_ptr<GLContext>& context);

		void Free();

		// px::Events...
		void DrawObjectRegisteredEvent(const px::DrawObject& drawObject);
		void DrawObjectUnregisteredEvent(const px::DrawObject& drawObject);
		void DrawObjectGeometryChangedEvent(const px::DrawObject& drawObject);
		void DrawObjectMaterialChangedEvent(const px::DrawObject& drawObject);
		void DrawObjectTransformChangedEvent(const px::DrawObject& drawObject);
		void TextureRegisteredEvent(const px::Texture& texture);
		void TextureUnregisteredEvent(const px::Texture& texture);
		void TextureChangedEvent(const px::Texture& texture);
		
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
		//std::shared_ptr<GLComputeContext> GetComputeContext();

		bool DebugTracing() { return debugTracing_; }
		void DebugTracing(bool trace) { debugTracing_ = trace; }

	private:
		// Our resource managers...
		std::list<std::shared_ptr<GLDrawObject>> drawObjects_;
		std::map<px::String, std::shared_ptr<GLTexture>> textures_;
		std::list<std::shared_ptr<GLContext>> contexts_;
		
		// Picker counter...
		unsigned int pickerCounter_ = 100;

		bool textureRecacheRequired_ = false;
		bool cleanupDestroyedRequired_ = false;

		bool debugTracing_ = false;
		int scheduledProcessPollRate_ = 200;

		// current context lock...
		std::mutex context_mutex_;									// Race condition for adding alinger event...
	};

	extern GLInstance* GetGLInstance();

}


#endif // AxWGL_INSTANCE_HPP
