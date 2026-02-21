#ifndef PXGL_TEXTURE_HPP
#define PXGL_TEXTURE_HPP

#include "GLFunctions.hpp"
#include <include/px.hpp>

namespace pxgl
{
	class GLTexture
	{
	public:

		enum DirtyState
		{
			NotDirty = 0,
			Dirty,
			Destroyed
		};

		GLTexture(const px::Texture* texture)
			: textureName_(texture->TextureName()), dirty_(DirtyState::Dirty), imageToCache_(texture->GetImageShared())
		{
		}

		void Free()
		{
			if (ID_)
				glDeleteTextures(1, &ID_);
			ID_ = 0;
			dirty_ = DirtyState::NotDirty;
		}

		void Cache()
		{
			if (imageToCache_)
			{
				Cache(*imageToCache_.get());
				imageToCache_ = std::shared_ptr<px::Image2DInterface>();
			}
		}

		void Cache(const px::Image2DInterface& image)
		{
			if (!ID_)
				glGenTextures(1, &ID_);

			if (ID_)
			{
				glBindTexture(GL_TEXTURE_2D, ID_);

				if (const px::ImageRGBA32* rgba32 = dynamic_cast<const px::ImageRGBA32*>(&image))
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.Raw());
				else if (const px::ImageRGBA128* rgba128 = dynamic_cast<const px::ImageRGBA128*>(&image))
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, image.Width(), image.Height(), 0, GL_RGBA, GL_FLOAT, image.Raw());
				else
					throw px::Exception(px::ErrorID::DisplayError, { { px::TagID::What, px::Str("Texture format not supported") } });

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping_);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping_);

				glBindTexture(GL_TEXTURE_2D, NULL);

				dirty_ = DirtyState::NotDirty;
			}
		}

		const px::String& TextureName() const { return textureName_; }
		void SetDirtyState(const DirtyState& state) { dirty_ = state; }
		void SetDirtyState(const std::shared_ptr<px::Image2DInterface>& image) { imageToCache_ = image; SetDirtyState(DirtyState::Dirty); }
		const DirtyState& GetDirtyState() const { return dirty_; }
		unsigned int GetTextureID() const { return ID_; }

	private:
		DirtyState dirty_;
		unsigned int ID_ = 0;
		unsigned int filtering_ = GL_LINEAR;
		unsigned int wrapping_ = GL_CLAMP_TO_EDGE;
		px::String textureName_;
		std::shared_ptr<px::Image2DInterface> imageToCache_;
	};

}

#endif // PXGL_TEXTURE_HPP