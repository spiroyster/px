#ifndef pxgl_GLCONTEXT_HPP
#define pxgl_GLCONTEXT_HPP

#include <memory>
#include <string>
#include <thread>

#define GLERRORCHECK ErrorCheck(__LINE__);

namespace pxgl
{
	class GLFunctions;
	class NonVisibleWindow;

	struct GLInfo
	{
		std::string vendor_;
		std::string renderer_;
		std::string version_;
		std::string GLSLVersion_;

		int maxTextureSize_;					// max texture dimension
		int maxTextureBufferSizeInBytes_;		// max texture buffer size in bytes
		int maxTexture3DSize_;					// max 3D texture dimension

		int maxComputeWorkGroupCount_[3];		// max number of workgroups (per dimension)
		int maxComputeWorkGroupSize_[3];		// max size of workgroup (per dimension)
		int maxComputeWorkGroupInvocations_;	// max number of workgroup invocations

		int maxShaderStorageBlockSizeInBytes_;	// max SSBO mem footprint
	};

	class GLContext
	{
		void Initialise();
	public:

		GLContext(const std::thread::id& id, void* dc, void* hglrc, const GLContext* parentContext);

		virtual ~GLContext();

		void Free();
		void ContextMakeCurrent() const;
		void ContextDisable() const;

		const GLInfo& Info() const { return info_; }
		const GLFunctions& Functions() const { return *functions_; }
		const std::string& ContextName() const { return name_; }
		const std::thread::id& GetThreadID() const { return threadID_; }
		//GLSLShaderManager& ShaderManager() const { return *shaderManager_; }
		//GLSLScheduler& Scheduler() { return *scheduler_; }

		void ErrorCheck(unsigned int line) const;

	protected:
		//std::unique_ptr<GLSLShaderManager> shaderManager_;
		//std::unique_ptr<GLSLScheduler> scheduler_;
		std::string name_;
		std::unique_ptr<GLFunctions> functions_;
		GLInfo info_;
		void* hglrc_ = nullptr;
		void* hDC_ = nullptr;
		std::shared_ptr<NonVisibleWindow> window_;
		std::thread::id threadID_;
	};

}

#endif // pxgl_GLCONTEXT_HPP
