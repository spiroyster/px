#ifndef PXGL_HPP
#define PXGL_HPP

#include "px.hpp"

namespace pxgl
{

	class Display : public px::Display
	{
	public:
		Display(const px::String& name, int width, int height);

		void Render(void* dc);
		void Resize(void* dc, unsigned int width, unsigned int height);

		Coordinate GetDisplayCoordinate(const CoordinateNormalised& dc);
		CoordinateNormalised GetDisplayCoordinateNormalised(const px::Display::Coordinate& dc);
		std::pair<px::View*, px::View::Coordinate> GetViewCoordinate(const Coordinate& dc);
		std::optional<px::Display::Coordinate> GetDisplayCoordinate(const px::View& view, const px::View::Coordinate& vc);
		std::shared_ptr<px::ImageRGBA32> FrameImage();

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		const px::DrawObject* Interrogate(const Display::Coordinate& xy) const;
		px::Parameters GetContextInfo() const;
	};

	std::shared_ptr<Display> CreateDisplay(int width, int height, void* dc);
	std::shared_ptr<Display> CreateDisplay(int width, int height, void* dc, void* hglrc);
}

#endif // PXGL_HPP