#ifndef PX_COLOUR_HPP
#define PX_COLOUR_HPP

#include "../Type.hpp"

#include <random>

namespace px
{
	/// <summary>
/// Representation of a 32-bit colour. This is RGBA (Red, Blue, Green and Alpha). 4 Channels, each 1 byte = 32-bit RGBA.
/// Each channel has a range of 0 <= r,g,b,a <= 255.
/// </summary>
	class RGBA32
	{
	public:

		/// <summary>
		/// Default constructor to translucent black (0,0,0,0).
		/// </summary>
		RGBA32() {}

		/// <summary>
		/// Constructor from red, green, blue and alpha values.
		/// </summary>
		/// <param name="r">The red value to construct with</param>
		/// <param name="g">The green value to construct with</param>
		/// <param name="b">The blue value to construct with</param>
		/// <param name="a">The alpha value to construct with</param>
		RGBA32(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : rgba_{ r, g, b, a } {}
		RGBA32(unsigned char pixel) : rgba_{ pixel, pixel, pixel, pixel } {}

		bool operator==(const RGBA32& rhs) const
		{
			return (rgba_[0] == rhs.rgba_[0] &&
				rgba_[1] == rhs.rgba_[1] &&
				rgba_[2] == rhs.rgba_[2] &&
				rgba_[3] == rhs.rgba_[3]);
		}

		bool operator!=(const RGBA32& rhs) const
		{
			return !(*this == rhs);
		}

		/// <summary>
		/// The number of channels of this colour representation.
		/// </summary>
		/// <returns>The channel count for this colour representation</returns>
		static unsigned int ChannelCount() { return 4; }

		/// <summary>
		/// The stride of each channel. This is the size in memory of a single channel (N.B A single pixel has ChannelCount() * Stride() memory footprint). 
		/// All channels in this representation are assumed to be the same size.
		/// </summary>
		/// <returns>The memory footprint of a single channel of this colour representation</returns>
		static unsigned int ChannelStride() { return sizeof(unsigned char); }

		/// <summary>
		/// Accessor for the Red channel component.
		/// </summary>
		/// <returns>The red component</returns>
		unsigned char R() const { return rgba_[0]; }

		/// <summary>
		/// Accessor for the Green channel component.
		/// </summary>
		/// <returns>The green component</returns>
		unsigned char G() const { return rgba_[1]; }

		/// <summary>
		/// Accessor for the Blue channel component.
		/// </summary>
		/// <returns>The blue component</returns>
		unsigned char B() const { return rgba_[2]; }

		/// <summary>
		/// Accessor for the Alpha channel component.
		/// </summary>
		/// <returns>The alpha component</returns>
		unsigned char A() const { return rgba_[3]; }

	private:
		unsigned char rgba_[4] = { 0, 0, 0, 0 };
	};

	class RGB24
	{
	public:

		/// <summary>
		/// Default constructor to translucent black (0,0,0,0).
		/// </summary>
		RGB24() {}

		/// <summary>
		/// Constructor from red, green, blue and alpha values.
		/// </summary>
		/// <param name="r">The red value to construct with</param>
		/// <param name="g">The green value to construct with</param>
		/// <param name="b">The blue value to construct with</param>
		/// <param name="a">The alpha value to construct with</param>
		RGB24(unsigned char r, unsigned char g, unsigned char b) : rgb_{ r, g, b } {}
		RGB24(unsigned char pixel) : rgb_{ pixel, pixel, pixel } {}

		bool operator==(const RGB24& rhs) const
		{
			return (rgb_[0] == rhs.rgb_[0] &&
				rgb_[1] == rhs.rgb_[1] &&
				rgb_[2] == rhs.rgb_[2]);
		}

		bool operator!=(const RGB24& rhs) const
		{
			return !(*this == rhs);
		}

		/// <summary>
		/// The number of channels of this colour representation.
		/// </summary>
		/// <returns>The channel count for this colour representation</returns>
		static unsigned int ChannelCount() { return 3; }

		/// <summary>
		/// The stride of each channel. This is the size in memory of a single channel (N.B A single pixel has ChannelCount() * Stride() memory footprint). 
		/// All channels in this representation are assumed to be the same size.
		/// </summary>
		/// <returns>The memory footprint of a single channel of this colour representation</returns>
		static unsigned int ChannelStride() { return sizeof(unsigned char); }

		/// <summary>
		/// Accessor for the Red channel component.
		/// </summary>
		/// <returns>The red component</returns>
		unsigned char R() const { return rgb_[0]; }

		/// <summary>
		/// Accessor for the Green channel component.
		/// </summary>
		/// <returns>The green component</returns>
		unsigned char G() const { return rgb_[1]; }

		/// <summary>
		/// Accessor for the Blue channel component.
		/// </summary>
		/// <returns>The blue component</returns>
		unsigned char B() const { return rgb_[2]; }

	private:
		unsigned char rgb_[3] = { 0, 0, 0 };
	};

	/// <summary>
	/// Representation of a 128-bit colour. This is RGBA (Red, Blue, Green and Alpha). 4 Channels, each 4 bytes = 128-bit RGBA.
	/// Each channel has a range of 0.0 <= r,g,b,a <= 1.0.
	/// </summary>
	class RGBA128
	{
	public:

		/// <summary>
		/// Default constructor to translucent black (0.0f,0.0f,0.0f,0.0f).
		/// </summary>
		RGBA128() {}

		/// <summary>
		/// Constructor from red, green, blue and alpha values.
		/// </summary>
		/// <param name="r">The red value to construct with</param>
		/// <param name="g">The green value to construct with</param>
		/// <param name="b">The blue value to construct with</param>
		/// <param name="a">The alpha value to construct with</param>
		RGBA128(float r, float g, float b, float a) : rgba_{ r, g, b, a } {}

		/// <summary>
		/// The number of channels of this colour representation.
		/// </summary>
		/// <returns>The channel count for this colour representation</returns>
		static unsigned int ChannelCount() { return 4; }

		/// <summary>
		/// The stride of each channel. This is the size in memory of a single channel (N.B A single pixel has ChannelCount() * Stride() memory footprint). 
		/// All channels in this representation are assumed to be the same size.
		/// </summary>
		/// <returns>The memory footprint of a single channel of this colour representation</returns>
		static unsigned int Stride() { return sizeof(float); }

		/// <summary>
		/// Accessor for the Red channel component.
		/// </summary>
		/// <returns>The red component</returns>
		float R() const { return rgba_[0]; }

		/// <summary>
		/// Accessor for the Green channel component.
		/// </summary>
		/// <returns>The green component</returns>
		float G() const { return rgba_[1]; }

		/// <summary>
		/// Accessor for the Blue channel component.
		/// </summary>
		/// <returns>The blue component</returns>
		float B() const { return rgba_[2]; }

		/// <summary>
		/// Accessor for the Alpha channel component.
		/// </summary>
		/// <returns>The alpha component</returns>
		float A() const { return rgba_[3]; }

	private:
		float rgba_[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	class RGB96
	{
	public:

		/// <summary>
		/// Default constructor to translucent black (0.0f,0.0f,0.0f,0.0f).
		/// </summary>
		RGB96() {}

		/// <summary>
		/// Constructor from red, green, blue and alpha values.
		/// </summary>
		/// <param name="r">The red value to construct with</param>
		/// <param name="g">The green value to construct with</param>
		/// <param name="b">The blue value to construct with</param>
		/// <param name="a">The alpha value to construct with</param>
		RGB96(float r, float g, float b) : rgb_{ r, g, b } {}

		/// <summary>
		/// The number of channels of this colour representation.
		/// </summary>
		/// <returns>The channel count for this colour representation</returns>
		static unsigned int ChannelCount() { return 3; }

		/// <summary>
		/// The stride of each channel. This is the size in memory of a single channel (N.B A single pixel has ChannelCount() * Stride() memory footprint). 
		/// All channels in this representation are assumed to be the same size.
		/// </summary>
		/// <returns>The memory footprint of a single channel of this colour representation</returns>
		static unsigned int Stride() { return sizeof(float); }

		/// <summary>
		/// Accessor for the Red channel component.
		/// </summary>
		/// <returns>The red component</returns>
		float R() const { return rgb_[0]; }

		/// <summary>
		/// Accessor for the Green channel component.
		/// </summary>
		/// <returns>The green component</returns>
		float G() const { return rgb_[1]; }

		/// <summary>
		/// Accessor for the Blue channel component.
		/// </summary>
		/// <returns>The blue component</returns>
		float B() const { return rgb_[2]; }

	private:
		float rgb_[3] = { 0.0f, 0.0f, 0.0f };
	};

	// Single colour...
	static RGBA128 Unique(int seed)
	{
		std::default_random_engine rng(seed);
		std::uniform_int_distribution<int> rng_dist(0, 255);
		return RGBA128(rng_dist(rng) / 255.0f, rng_dist(rng) / 255.0f, rng_dist(rng) / 255.0f, 1.0f);
	}

	static RGBA128 Normal(const float& x, const float& y, const float& z)
	{
		return RGBA128((x * 0.5f) + 0.5f, (y * 0.5f) + 0.5f, (z * 0.5f) + 0.5f, 1.0f);
	}

	// Convert....
	static RGBA32 Convert(const RGBA128& deepColour)
	{
		return RGBA32(static_cast<int>(deepColour.R() * 255.0), static_cast<int>(deepColour.G() * 255.0), static_cast<int>(deepColour.B() * 255.0), static_cast<int>(deepColour.A() * 255.0));
	}

	static RGBA128 Convert(const RGBA32& trueColour)
	{
		return RGBA128(static_cast<float>(trueColour.R()) / 255.0f, static_cast<float>(trueColour.G()) / 255.0f, static_cast<float>(trueColour.B()) / 255.0f, static_cast<float>(trueColour.A()) / 255.0f);
	}

	static RGB24 Convert(const RGB96& deepColour)
	{
		return RGB24(static_cast<int>(deepColour.R() * 255.0), static_cast<int>(deepColour.G() * 255.0), static_cast<int>(deepColour.B() * 255.0));
	}

	static RGB96 Convert(const RGB24& trueColour)
	{
		return RGB96(static_cast<float>(trueColour.R()) / 255.0f, static_cast<float>(trueColour.G()) / 255.0f, static_cast<float>(trueColour.B()) / 255.0f);
	}

}

#endif // PX_COLOUR_HPP