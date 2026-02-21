#ifndef PX_DISPLAY_HPP
#define PX_DISPLAY_HPP

#include "Type.hpp"
#include "Draw/View.hpp"
#include "Geometry/Vector.hpp"

#include <memory>

namespace px
{
	// display
	class Display
	{
	public:
		Display(const String& name)
			: name_(name)
		{
		}

		typedef px::Coordinate Coordinate;
		typedef px::Geometry::Vector2 CoordinateNormalised;
		
		virtual void Render(void* dc) = 0;
		virtual void Resize(void* dc, unsigned int width, unsigned int height) = 0;

		virtual Coordinate GetDisplayCoordinate(const CoordinateNormalised& dc) = 0;
		virtual CoordinateNormalised GetDisplayCoordinateNormalised(const Coordinate& dc) = 0;
		virtual std::pair<View*, View::Coordinate> GetViewCoordinate(const Coordinate& dc) = 0;
		virtual std::optional<Coordinate> GetDisplayCoordinate(const View& view, const View::Coordinate& vc) = 0;
		virtual std::shared_ptr<ImageRGBA32> FrameImage() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual const DrawObject* Interrogate(const Display::Coordinate& xy) const = 0;
		virtual Parameters GetContextInfo() const = 0;
		 
		//virtual void SetDisplayID(int displayID) { displayID_ = displayID; }
		//virtual int GetDisplayID() { return displayID_; }

		const String& displayName() const { return name_; }

	private:
		String name_;
	};

	// Set this display (adaptor)
	void SetDisplay(Display* display); 

	// Get display?
}


#endif // PX_DISPLAY_HPP