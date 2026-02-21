#include "GLFunctions.hpp"
#include "GLContext.hpp"

#include <Windows.h>
#include <gl/GL.h>
#include "wglext.h"

#include <include/px.hpp>
#include <include/px/String.hpp>

namespace pxgl
{
	namespace
	{
		std::string GetThreadIDAsString(const std::thread::id& threadID)
		{
			return std::to_string(static_cast<unsigned int>(std::hash<std::thread::id>{}(threadID)));
		}

		static WNDCLASSEX wcex;

		HGLRC CreateContext(void* hdc, int major, int minor)
		{
			HDC hDC = static_cast<HDC>(hdc);
			HGLRC result = NULL;

			// first check we have a valid device context
			if (!hDC)
				throw px::Exception(px::ErrorID::DisplayError,
					{
						{ px::TagID::What, px::Str("No valid device context")},
						{ px::TagID::Callee, px::Str(__FUNCTION__)}
					});

			HGLRC tempContext = wglCreateContext(hDC);
			wglMakeCurrent(hDC, tempContext);

			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, major,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor,
				WGL_CONTEXT_FLAGS_ARB, 0,
				0
			};

			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			if (wglCreateContextAttribsARB)
			{
				result = wglCreateContextAttribsARB(hDC, 0, attribs);
				wglMakeCurrent(NULL, NULL);
				wglDeleteContext(tempContext);
				if (!wglMakeCurrent(hDC, result))
					throw px::Exception(px::ErrorID::DisplayError,
						{
							{ px::TagID::Callee, px::Str(__FUNCTION__)}
						});
			}

			return result;
		}

		void SetPFD(HDC hDC)
		{
			// This should happend once for the main draw thread since this context is never destroyed...
			PIXELFORMATDESCRIPTOR pfd;
			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 24; // 8?
			pfd.cStencilBits = 8;
			pfd.iLayerType = PFD_MAIN_PLANE;

			int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
			if (nPixelFormat == 0)
				throw px::Exception(px::ErrorID::DisplayError,
					{
						{ px::TagID::What, px::Str("Unable to choose pixel format.")},
						{ px::TagID::Result, nPixelFormat},
						{ px::TagID::Callee, px::Str(__FUNCTION__)}
					});

			if (!SetPixelFormat(hDC, nPixelFormat, &pfd))
				throw px::Exception(px::ErrorID::DisplayError,
					{
						{ px::TagID::What, px::Str("Unable to set pixel format.")},
						{ px::TagID::Result, nPixelFormat},
						{ px::TagID::Callee, px::Str(__FUNCTION__)}
					});
		}
	}

	class NonVisibleWindow
	{
		std::wstring name_;
		std::wstring title_;
		HWND hWnd_;
	public:
		NonVisibleWindow(HINSTANCE hInst, const std::wstring& name)
			: name_(name), title_(name)
		{
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc = DefWindowProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInst;
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = name_.c_str();

			if (!RegisterClassEx(&wcex))
			{
				std::ostringstream oss;
				oss << GetLastError();
				throw px::Exception(px::ErrorID::DisplayError,
					{
						{ px::TagID::What, px::Str(oss.str())},
						{ px::TagID::Callee, px::Str(__FUNCTION__)}
					});
			}

			hWnd_ = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, name_.c_str(), title_.c_str(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL, NULL, hInst, NULL);

			if (!hWnd_)
			{
				std::ostringstream oss;
				oss << GetLastError();
				throw px::Exception(px::ErrorID::DisplayError,
					{
						{ px::TagID::What, px::Str(oss.str())},
						{ px::TagID::Callee, px::Str(__FUNCTION__)}
					});
			}
		}

		~NonVisibleWindow()
		{
			DestroyWindow(hWnd_);
		}

		HDC GetHDC() { return GetDC(hWnd_); }
	};

	GLContext::GLContext(const std::thread::id& id, void* dc, void* hglrc, const GLContext* parentContext)
		: threadID_(id), hDC_(dc), hglrc_(hglrc)
	{
		if (!hglrc_)
		{
			// If dc, this is constructing draw context, if not create a nonvisible window to use its draw context...
			if (!hDC_)
			{
				// Create the non-visible window (device context)
				window_ = std::make_shared<NonVisibleWindow>(GetModuleHandle(NULL), px::Str(GetThreadIDAsString(threadID_)));
				hDC_ = window_->GetHDC();
			}

			SetPFD(static_cast<HDC>(hDC_));

			// We need to create a new context...
			hglrc_ = CreateContext(hDC_, 4, 3);
			if (!hglrc_)
				throw px::Exception(px::ErrorID::DisplayError,
					{
						{ px::TagID::Callee, px::Str(__FUNCTION__)}
					});
		}

		if (parentContext)
			wglShareLists(static_cast<HGLRC>(parentContext->hglrc_), static_cast<HGLRC>(hglrc_));

		Initialise();
		px::Message(px::Str("[GL] Created context (" + GetThreadIDAsString(threadID_) + ")."));
	}

	void GLContext::Initialise()
	{
		// Get the GL functions...
		functions_ = std::make_unique<GLFunctions>();

		// Grab the GL info...
		info_.version_ = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		info_.vendor_ = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		info_.renderer_ = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		info_.GLSLVersion_ = std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info_.maxTextureSize_);
		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &info_.maxTextureBufferSizeInBytes_);
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &info_.maxTexture3DSize_);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &info_.maxShaderStorageBlockSizeInBytes_);
		for (int idx = 0; idx < 3; idx++)
		{
			functions_->glGetIntegeri_v_(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &info_.maxComputeWorkGroupCount_[idx]);
			functions_->glGetIntegeri_v_(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &info_.maxComputeWorkGroupSize_[idx]);
		}
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &info_.maxComputeWorkGroupInvocations_);
	}

	GLContext::~GLContext()
	{
		Free();
	}

	void GLContext::Free()
	{
		// Destroy the glrc context...
		if (static_cast<HGLRC>(hglrc_))
			wglDeleteContext(static_cast<HGLRC>(hglrc_));

		px::Message(px::Str("[GL] Destroyed context (" + GetThreadIDAsString(threadID_) + ")."));
	}

	void GLContext::MakeThisContextCurrent() const
	{
		if (static_cast<HDC>(hDC_))
		{
			if (wglMakeCurrent(static_cast<HDC>(hDC_), static_cast<HGLRC>(hglrc_)) == FALSE)
				px::Message(px::Str("Display Context Unable to make current."));
		}
	}

	void GLContext::ErrorCheck(unsigned int line) const
	{
		auto err = glGetError();
		while (err != GL_NO_ERROR)
		{
			px::Message(px::Str(std::to_string(line) + ": GL_ERROR " + std::to_string(err)));
			err = glGetError();
		}
	}

}


//#include "GLContext.hpp"
//
//#include <Windows.h>
//#include <gl/GL.h>
//#include "wglext.h"
//#include <sstream>
//
//#include <AxW/Message.hpp>
//#include <AxW/Util/String.hpp>
//
//namespace AxWGL
//{
//	namespace
//	{
//		static WNDCLASSEX wcex;
//
//
//
//		HGLRC CreateContext(void* hdc, int major, int minor)
//		{
//			HDC hDC = static_cast<HDC>(hdc);
//			HGLRC result = NULL;
//
//			// first check we have a valid device context
//			if (!hDC)
//				throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//					{
//						{ px::Error::Tag::What, px::Str("No valid device context")},
//						{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//					});
//
//			HGLRC tempContext = wglCreateContext(hDC);
//			wglMakeCurrent(hDC, tempContext);
//
//			int attribs[] =
//			{
//				WGL_CONTEXT_MAJOR_VERSION_ARB, major,
//				WGL_CONTEXT_MINOR_VERSION_ARB, minor,
//				WGL_CONTEXT_FLAGS_ARB, 0,
//				0
//			};
//
//			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
//			if (wglCreateContextAttribsARB)
//			{
//				result = wglCreateContextAttribsARB(hDC, 0, attribs);
//				wglMakeCurrent(NULL, NULL);
//				wglDeleteContext(tempContext);
//				if (!wglMakeCurrent(hDC, result))
//					throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//						{
//							{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//						});
//			}
//
//			return result;
//		}
//
//		void SetPFD(HDC hDC)
//		{
//			// This should happend once for the main draw thread since this context is never destroyed...
//			PIXELFORMATDESCRIPTOR pfd;
//			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
//			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
//			pfd.nVersion = 1;
//			pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
//			pfd.iPixelType = PFD_TYPE_RGBA;
//			pfd.cColorBits = 32;
//			pfd.cDepthBits = 24; // 8?
//			pfd.cStencilBits = 8;
//			pfd.iLayerType = PFD_MAIN_PLANE;
//
//			int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
//			if (nPixelFormat == 0)
//				throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//					{
//						{ px::Error::Tag::What, px::Str("Unable to choose pixel format.")},
//						{ px::Error::Tag::Result, px::Str(nPixelFormat)},
//						{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//					});
//
//			if (!SetPixelFormat(hDC, nPixelFormat, &pfd))
//				throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//					{
//						{ px::Error::Tag::What, px::Str("Unable to set pixel format.")},
//						{ px::Error::Tag::Result, px::Str(nPixelFormat)},
//						{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//					});
//		}
//
//		std::string GetThreadIDAsString(const std::thread::id& threadID)
//		{
//			return std::to_string(static_cast<unsigned int>(std::hash<std::thread::id>{}(threadID)));
//		}
//	}
//
//	class NonVisibleWindow
//	{
//		std::wstring name_;
//		std::wstring title_;
//		HWND hWnd_;
//	public:
//		NonVisibleWindow(HINSTANCE hInst, const std::wstring& name)
//			: name_(name), title_(name)
//		{
//			wcex.cbSize = sizeof(WNDCLASSEX);
//			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//			wcex.lpfnWndProc = DefWindowProc;
//			wcex.cbClsExtra = 0;
//			wcex.cbWndExtra = 0;
//			wcex.hInstance = hInst;
//			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//			wcex.lpszMenuName = NULL;
//			wcex.lpszClassName = name_.c_str();
//
//			if (!RegisterClassEx(&wcex))
//			{
//				std::ostringstream oss;
//				oss << GetLastError();
//				throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//					{
//						{ px::Error::Tag::What, px::Str(oss.str())},
//						{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//					});
//			}
//
//			hWnd_ = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, name_.c_str(), title_.c_str(), WS_OVERLAPPEDWINDOW,
//				CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL, NULL, hInst, NULL);
//
//			if (!hWnd_)
//			{
//				std::ostringstream oss;
//				oss << GetLastError();
//				throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//					{
//						{ px::Error::Tag::What, px::Str(oss.str())},
//						{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//					});
//			}
//		}
//
//		~NonVisibleWindow()
//		{
//			DestroyWindow(hWnd_);
//		}
//
//		HDC GetHDC() { return GetDC(hWnd_); }
//	};
//
//
//	GLContext::GLContext(const std::thread::id& id, void* dc, const GLContext* parentContext)
//		: threadID_(id), hDC_(static_cast<HDC>(dc))
//	{
//		// If dc, this is constructing draw context, if not create a nonvisible window to use its draw context...
//		if (!hDC_)
//		{
//			// Create the non-visible window (device context)
//			window_ = std::make_shared<NonVisibleWindow>(GetModuleHandle(NULL), px::Util::AnsiToWide(GetThreadIDAsString(threadID_)));
//			hDC_ = window_->GetHDC();
//		}
//
//		SetPFD(hDC_);
//
//		// We need to create a new context...
//		hglrc_ = CreateContext(hDC_, 4, 3);
//		if (!hglrc_)
//			throw px::Exception(px::Error::ID::DisplayUnableToCreate,
//				{
//					{ px::Error::Tag::Callee, px::Str(__FUNCTION__)}
//				});
//
//		if (parentContext)
//			wglShareLists(parentContext->hglrc_, hglrc_);
//
//		// Get the GL functions...
//		functions_ = std::make_unique<GLFunctions>();
//
//		// Grab the GL info...
//		info_.version_ = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
//		info_.vendor_ = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
//		info_.renderer_ = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
//		info_.GLSLVersion_ = std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
//		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &info_.maxTextureSize_);
//		glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &info_.maxTextureBufferSizeInBytes_);
//		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &info_.maxTexture3DSize_);
//		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &info_.maxShaderStorageBlockSizeInBytes_);
//		for (int idx = 0; idx < 3; idx++)
//		{
//			functions_->glGetIntegeri_v_(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &info_.maxComputeWorkGroupCount_[idx]);
//			functions_->glGetIntegeri_v_(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &info_.maxComputeWorkGroupSize_[idx]);
//		}
//		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &info_.maxComputeWorkGroupInvocations_);
//
//		shaderManager_ = std::make_unique<GLShaderManager>(*this);
//		//scheduler_ = std::make_unique<GLSLScheduler>(*this, shaderManager_->ActiveThreadCount());
//
//		px::Message("[GL] Created context (" + GetThreadIDAsString(threadID_) + ").");
//	}
//
//	GLContext::~GLContext()
//	{
//		Free();
//	}
//
//	void GLContext::Free()
//	{
//		// Destroy the glrc context...
//		if (hglrc_)
//			wglDeleteContext(hglrc_);
//
//		px::Message("[GL] Destroyed context (" + GetThreadIDAsString(threadID_) + ").");
//	}
//
//	void GLContext::MakeThisContextCurrent() const
//	{
//		if (hDC_)
//		{
//			if (wglMakeCurrent(hDC_, hglrc_) == FALSE)
//				px::Message("Display Context Unable to make current.");
//		}
//	}
//
//	void GLContext::ErrorCheck(unsigned int line) const
//	{
//		auto err = glGetError();
//		while (err != GL_NO_ERROR)
//		{
//			px::Message(std::to_string(line) + ": GL_ERROR " + std::to_string(err));
//			err = glGetError();
//		}
//	}
//}
