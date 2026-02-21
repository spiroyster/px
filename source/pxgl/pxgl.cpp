#include <include/pxgl.hpp>

#include "GLInstance.hpp"
#include "GLContext.hpp"

#include "thirdparty/glo/include/glo/glofb.hpp"

namespace pxgl
{
	namespace
	{
		static std::unique_ptr<GLInstance> instance_;
	}

	GLInstance* GetGLInstance()
	{
		if (!instance_)
			throw px::Exception(px::ErrorID::DisplayError,
				{
					{ px::TagID::What, px::Str("Unable to initialise GL.") },
					{ px::TagID::Callee, px::Str(__FUNCTION__) }
				});

		return instance_.get();
	}

	std::shared_ptr<Display> CreateDisplay(int width, int height, void* dc)
	{
		if (!instance_)
			instance_ = std::make_unique<GLInstance>(std::make_shared<GLContext>(std::this_thread::get_id(), dc, nullptr, nullptr));
		return std::make_shared<Display>(px::Str("pxgl"), width, height);
	}

	std::shared_ptr<Display> CreateDisplay(int width, int height, void* dc, void* hglrc)
	{
		if (!instance_)
			instance_ = std::make_unique<GLInstance>(std::make_shared<GLContext>(std::this_thread::get_id(), dc, hglrc, nullptr));
		return std::make_shared<Display>(px::Str("pxgl"), width, height);
	}


	namespace
	{
		// gBuffer...
		static std::shared_ptr<glo::frame_buffer> gBuffer_;

		// GUI buffer...
		static std::shared_ptr<glo::frame_buffer> guiBuffer_;

		// framebuffer...
		static std::shared_ptr<glo::frame_buffer> frameBuffer_;
	}

	Display::Display(const px::String& name, int width, int height)
		:	px::Display(name)
	{
		// Create our framebuffers
		//throw px::Exception(px::ErrorID::NYI);
	}

	void Display::Render(void* dc)
	{
		//
		//throw px::Exception(px::ErrorID::NYI);
	}

	void Display::Resize(void* dc, unsigned int width, unsigned int height)
	{
		// Resize the buffers...


		// Resize the views...

	}

	px::Display::Coordinate Display::GetDisplayCoordinate(const px::Display::CoordinateNormalised& dc)
	{
		return
		{
			static_cast<int>(dc.x_ * static_cast<float>(frameBuffer_->width())),
			static_cast<int>(dc.y_ * static_cast<float>(frameBuffer_->height())),
		};
	}

	px::Display::CoordinateNormalised Display::GetDisplayCoordinateNormalised(const px::Display::Coordinate& dc)
	{
		return
		{
			static_cast<float>(dc.x_) / static_cast<float>(frameBuffer_->width()),
			static_cast<float>(dc.y_) / static_cast<float>(frameBuffer_->height()),
		};
	}

	std::pair<px::View*, px::View::Coordinate> Display::GetViewCoordinate(const px::Display::Coordinate& dc)
	{
		throw px::Exception(px::ErrorID::NYI);
	}

	std::optional<px::Display::Coordinate> Display::GetDisplayCoordinate(const px::View& view, const px::View::Coordinate& vc)
	{
		throw px::Exception(px::ErrorID::NYI);
	}

	std::shared_ptr<px::ImageRGBA32> Display::FrameImage()
	{
		throw px::Exception(px::ErrorID::NYI);
	}

	unsigned int Display::GetWidth() const
	{
		return frameBuffer_->width();
	}

	unsigned int Display::GetHeight() const
	{
		return frameBuffer_->height();
	}

	const px::DrawObject* Display::Interrogate(const px::Display::Coordinate& xy) const
	{
		// Use picker buffer in gBuffer...
		throw px::Exception(px::ErrorID::NYI);
	}

	px::Parameters Display::GetContextInfo() const
	{
		auto context = GetGLInstance()->GetMainThreadContext();
		return
		{
			{ px::Str("DisplayName"), px::Str(context->GetContext().ContextName()) },
			{ px::Str("GL_VERSION"), px::Str(context->GetContext().Info().version_) },
			{ px::Str("GL_VENDOR"), px::Str(context->GetContext().Info().vendor_) },
			{ px::Str("GL_RENDERER"), px::Str(context->GetContext().Info().renderer_) },
			{ px::Str("GL_SHADING_LANGUAGE_VERSION"), px::Str(context->GetContext().Info().GLSLVersion_) },
			{ px::Str("GL_MAX_TEXTURE_SIZE"), context->GetContext().Info().maxTextureSize_ },
			{ px::Str("GL_MAX_TEXTURE_BUFFER_SIZE"), context->GetContext().Info().maxTextureBufferSizeInBytes_ },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_COUNT_X"), context->GetContext().Info().maxComputeWorkGroupCount_[0] },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_COUNT_Y"), context->GetContext().Info().maxComputeWorkGroupCount_[1] },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_COUNT_Z"), context->GetContext().Info().maxComputeWorkGroupCount_[2] },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_SIZE_X"), context->GetContext().Info().maxComputeWorkGroupSize_[0] },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_SIZE_Y"), context->GetContext().Info().maxComputeWorkGroupSize_[1] },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_SIZE_Z"), context->GetContext().Info().maxComputeWorkGroupSize_[2] },
			{ px::Str("GL_MAX_SHADER_STORAGE_BLOCK_SIZE"), context->GetContext().Info().maxShaderStorageBlockSizeInBytes_ },
			{ px::Str("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS"), context->GetContext().Info().maxComputeWorkGroupInvocations_ }
		};
	}
}