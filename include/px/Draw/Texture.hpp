#ifndef PX_TEXTURE_HPP
#define PX_TEXTURE_HPP

#include "Image.hpp"

namespace px
{
	// Special type of data container...
	class Texture
	{
	public:

		/// <summary>
		/// Constructor with name and image.
		/// </summary>
		/// <param name="name">The unique name of this texture</param>
		/// <param name="image">Handle to the image data for this texture to use</param>
		Texture(const String& name, const std::shared_ptr<Image2DInterface>& image);

		/// <summary>
		/// Destrcutor unregisteres itself from the scene manager.
		/// </summary>
		virtual ~Texture();

		/// <summary>
		/// Unique name of this texture.
		/// </summary>
		/// <returns>The name of this texture</returns>
		const String& TextureName() const { return name_; }

		/// <summary>
		/// Get the image that this texutre uses.
		/// </summary>
		/// <returns>The image of this texture</returns>
		const Image2DInterface& GetImage() const { return *image_; }
		const std::shared_ptr<Image2DInterface>& GetImageShared() const { return image_; }

		void SetImage(const std::shared_ptr<Image2DInterface>& image);

	private:
		std::shared_ptr<Image2DInterface> image_;
		String name_;
	};
}

#endif // PX_TEXTURE_HPP