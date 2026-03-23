#ifndef PX_IMAGE_HPP
#define PX_IMAGE_HPP

#include "Colour.hpp"

#include <memory>

namespace px
{
	
	/// <summary>
	/// 2D Image interface for all 2D images. Image data should be stored (and acessable) as a continuous byte array (Event if the channel type is float, aka 96/128 bit)
	/// </summary>
	class Image2DInterface
	{
	public:

		/// <summary>
		/// The width of the image.
		/// </summary>
		/// <returns>The width of the image</returns>
		virtual unsigned int Width() const = 0;

		/// <summary>
		/// The height of the image.
		/// </summary>
		/// <returns>The height of the image</returns>
		virtual unsigned int Height() const = 0;

		/// <summary>
		/// The number of pixels in this image.
		/// </summary>
		/// <returns>The number of pixels in the image</returns>
		unsigned int Count() const { return Width() * Height(); }

		/// <summary>
		/// Immutable pointer to the start of the raw byte array.
		/// </summary>
		/// <returns>The immutable start of the image byte array</returns>
		virtual const void* Raw() const = 0;

		/// <summary>
		/// Mmutable pointer to the start of the raw byte array.
		/// </summary>
		/// <returns>The mutable start of the image byte array</returns>
		virtual void* Raw() = 0;

		/// <summary>
		/// Flip the image data horizontally.
		/// </summary>
		virtual void FlipH() = 0;

		/// <summary>
		/// Flip the image data vertically.
		/// </summary>
		virtual void FlipV() = 0;

		/// <summary>
		/// Perform a deep copy of this image.
		/// </summary>
		/// <returns>A copy of this image</returns>
		virtual std::shared_ptr<Image2DInterface> DeepCopy() const = 0;
	};

	/// <summary>
	/// Template class for a 2D image.
	/// </summary>
	/// <typeparam name="T">The type of colour component to use for each pixel of the image</typeparam>
	template<class T>
	class Image2D : public Image2DInterface
	{
	public:

		/// <summary>
		/// Constructor with just width and height. Byte array is allocated, but unintialised.
		/// </summary>
		/// <param name="width">The width of the image to construct</param>
		/// <param name="height">The height of the image to construct</param>
		Image2D(unsigned int width, unsigned int height) : width_(width), height_(height), bits_(width* height) {}

		/// <summary>
		/// Constructor from width, height and byte array. The size of the byte array must be equal to the image count (width * height).
		/// </summary>
		/// <param name="width">The width of the image to construct</param>
		/// <param name="height">The height of the image to construct</param>
		/// <param name="bits">Raw byte array to construct image with</param>
		Image2D(unsigned int width, unsigned int height, const std::vector<T>& bits) : width_(width), height_(height), bits_(bits) {}

		unsigned int Width() const { return width_; }
		unsigned int Height() const { return height_; }

		/// <summary>
		/// Get the immutable pixel at x,y in this image.
		/// </summary>
		/// <param name="x">The x coordinate to query</param>
		/// <param name="y">The y coordinate to query</param>
		/// <returns>The immutable pixel at xy</returns>
		const T* Pixel(unsigned int x, unsigned int y) const { return &bits_[(y * width_) + x]; }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="n"></param>
		/// <returns></returns>
		const T* Pixel(unsigned int n) const { return &bits_[n]; }

		/// <summary>
		/// Get the mutable pixel at x,y in this image.
		/// </summary>
		/// <param name="x">The x coordinate to query</param>
		/// <param name="y">The y coordinate to query</param>
		/// <returns>The mutable pixel at xy</returns>
		T* Pixel(unsigned int x, unsigned int y) { return &bits_[(y * width_) + x]; }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="n"></param>
		/// <returns></returns>
		T* Pixel(unsigned int n) { return &bits_[n]; }

		const void* Raw() const { return static_cast<const void*>(Pixel(0, 0)); }
		void* Raw() { return static_cast<void*>(Pixel(0, 0)); }
		std::shared_ptr<Image2DInterface> DeepCopy() const
		{
			auto img = std::make_shared<Image2D<T>>(width_, height_);
			img->bits_ = bits_;
			return img;
		}
		void FlipH()
		{
			std::vector<T> newBits = bits_;
			for (unsigned int y = 0; y < height_; ++y)
			{
				for (unsigned int x = 0; x < width_; ++x)
					newBits[(y * width_) + x] = bits_[(y * width_) + ((width_ - 1) - x)];
			}
			std::swap(newBits, bits_);
		}
		void FlipV()
		{
			std::vector<T> newBits = bits_;
			for (unsigned int y = 0; y < height_; ++y)
			{
				for (unsigned int x = 0; x < width_; ++x)
					newBits[(y * width_) + x] = bits_[((height_ - y - 1) * width_) + x];
			}
			std::swap(newBits, bits_);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		const std::vector<T>& Bits() const { return bits_; }

	protected:
		unsigned int width_ = 0, height_ = 0;
		std::vector<T> bits_;
	};

	/// <summary>
	/// 32-bit 2D Image (float, 4 byte per channel, RGBA True Colour)
	/// </summary>
	typedef Image2D<RGBA32> ImageRGBA32;

	/// <summary>
	/// 
	/// </summary>
	typedef Image2D<RGB24> ImageRGB24;

	/// <summary>
	/// 128-bit 2D Image (float, 4 byte per channel, RGBA Deep Colour)
	/// </summary>
	typedef Image2D<RGBA128> ImageRGBA128;

	/// <summary>
	/// 
	/// </summary>
	typedef Image2D<RGB96> ImageRGB96;

	//class Font;

	namespace Image
	{
		/// <summary>
		/// 
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="colour"></param>
		/// <returns></returns>
		static std::shared_ptr<ImageRGBA32> SingleColour(unsigned int width, unsigned int height, const RGBA32& colour)
		{
			return std::make_shared<ImageRGBA32>(width, height, std::vector<RGBA32>(width * height, colour));
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="image"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <returns></returns>
		static std::shared_ptr<ImageRGBA32> Resize(const ImageRGBA32& image, unsigned int width, unsigned int height)
		{

		}

		static std::shared_ptr<ImageRGBA32> Crop(const ImageRGBA32& image, int x, int y, unsigned int width, unsigned int height, const RGBA32& background)
		{
			auto result = SingleColour(width, height, background);

			for (int yy = 0; yy < static_cast<int>(height); ++yy)
				for (int xx = 0; xx < static_cast<int>(width); ++xx)
				{
					int imgX = x + xx;
					int imgY = y + yy;

					if (imgX >= 0 && imgX < static_cast<int>(image.Width()) && imgY >= 0 && imgY < static_cast<int>(image.Height()))
						*result->Pixel(xx, yy) = *image.Pixel(imgX, imgY);
				}

			return result;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="colourA"></param>
		/// <param name="uCount"></param>
		/// <param name="colourB"></param>
		/// <param name="vCount"></param>
		/// <returns></returns>
		static std::shared_ptr<ImageRGBA32> CheckerBoard(unsigned int width, unsigned int height, const RGBA32& colourA, unsigned int uCount, const RGBA32& colourB, unsigned int vCount)
		{
			unsigned int uStride = width / uCount;
			unsigned int vStride = height / vCount;
			auto img = std::make_shared<ImageRGBA32>(width, height);
			RGBA32* pixels = img->Pixel(0, 0);
			for (unsigned int p = 0; p < img->Count(); ++p)
			{
				bool useColourA = true;
				unsigned int x = p % width;
				unsigned int y = static_cast<unsigned int>(p / width);
				if ((x / uStride) % 2)
					useColourA = !useColourA;
				if ((y / vStride) % 2)
					useColourA = !useColourA;
				*(pixels + p) = useColourA ? colourA : colourB;
			}
			return img;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <returns></returns>
		std::shared_ptr<ImageRGBA32> Noise(unsigned int width, unsigned int height);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="barcode"></param>
		/// <param name="border"></param>
		/// <param name="active"></param>
		/// <param name="inactive"></param>
		/// <param name="quiet"></param>
		/// <returns></returns>
		//std::shared_ptr<ImageRGBA32> Barcode(const Barcode& barcode, int border, const RGBA32& active, const RGBA32& inactive, const RGBA32& quiet, int scaling);

		/// <summary>
		/// 
		/// </summary>
		enum TextAlignment
		{
			Centre,
			Left,
			Right,
			Top,
			Bottom
		};

		/// <summary>
		/// 
		/// </summary>
		/// <param name="font"></param>
		/// <param name="text"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="horizontalAlignment"></param>
		/// <param name="verticalAlignment"></param>
		/// <returns></returns>
		//std::shared_ptr<ImageRGBA32> Text(const Font& font, const String& text, int width, int height, const TextAlignment& horizontalAlignment, const TextAlignment& verticalAlignment);
	}
}

#endif // PXG_IMAGE_HPP