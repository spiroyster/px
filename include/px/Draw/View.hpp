#ifndef PX_VIEW_HPP
#define PX_VIEW_HPP

#include "../Geometry/Vector.hpp"

#include <memory>

namespace px
{
	class DrawObject;

	class View
	{
	public:

		typedef px::Coordinate Coordinate;
		typedef px::Geometry::Vector2 CoordinateNormalised;

		class ActionInterface
		{
		public:
			ActionInterface(const View::Coordinate& xy) : lastMouse_(xy) {}

			static const unsigned int CONSUME = 0;
			static const unsigned int PROPAGATE = 1;

			virtual bool MouseMove(View& view, int button, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual bool MouseDown(View& view, int button, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual bool MouseUp(View& view, int button, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual bool MouseClick(View& view, int button, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual bool MouseWheel(View& view, int value, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual bool KeyUp(View& view, int key, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual bool KeyDown(View& view, int key, const View::Coordinate& xy) { lastMouse_ = xy; return PROPAGATE; }
			virtual const String ActionInterfaceName() const = 0;
			const View::Coordinate& LastMouse() const { return lastMouse_; }

			virtual void Draw(View& view) {}

		private:
			View::Coordinate lastMouse_;
		};

		View(const String& name);
		virtual ~View();

		struct Camera
		{
			/// <summary>
			/// The camera position.
			/// </summary>
			Geometry::Vector3 position_;

			Geometry::Vector3 out_;
			Geometry::Vector3 up_;
			Geometry::Vector3 along_;

			/// <summary>
			/// The focal point of the camera.
			/// </summary>
			Geometry::Vector3 focalPoint_;

			/// <summary>
			/// The width of the camera viewport in world space.
			/// </summary>
			float width_;

			/// <summary>
			/// The height of the camera viewport in world space.
			/// </summary>
			float height_;

			/// <summary>
			/// The near clipping distance of the camera frustum.
			/// </summary>
			float near_;

			/// <summary>
			/// The far clipping distance of the camera frustum.
			/// </summary>
			float far_;

			/// <summary>
			/// The field of view of this camera.
			/// </summary>
			float fov_;
		};

		// View attributes...
		const String& ViewName() const { return viewName_; }
		unsigned int ViewWidth() const { return viewWidth_; }
		unsigned int ViewHeight() const { return viewHeight_; }
		void Resize(unsigned int width, unsigned int height);

		// Camera operations...
		const Camera& CameraImmutable() const { return camera_; }
		Camera& CameraMutable() { return camera_; }

		// Operations... 
		virtual void Layout() {}
		virtual void Show(int displayID);
		virtual void Hide();

		// Query coordinate operations
		//virtual const DrawObject* Interrogate(const Coordinate& xy) const;

		//std::optional<px::Display::Coordinate> GetDisplayCoordinate(const Coordinate& vc) const;
		//std::optional<Coordinate> GetViewCoordinate(const px::Display::Coordinate& dc) const;
		//CoordinateNormalised GetViewCoordinateNormalised(const Coordinate& vc) const;
		//Coordinate GetViewCoordinate(const CoordinateNormalised& vcNormalised) const;

		// ActionInterface
		// View action interfaces...
		virtual void PushActionInterface(std::shared_ptr<ActionInterface> dai);
		virtual void PopActionInterface(const String& name);
		const std::list<std::shared_ptr<ActionInterface>>& ActionInterfaceStack() const { return actionInterfaceStack_; }

	protected:
		// UI Stack
		std::list<std::shared_ptr<ActionInterface>> actionInterfaceStack_;

	private:
		String viewName_;
		unsigned int viewWidth_;
		unsigned int viewHeight_;

		// Camera...
		Camera camera_;

		// list of scene objects
		std::list<std::shared_ptr<DrawObject>> drawObjects_;

		// List of controls?


	};

}

#endif // PX_VIEW_HPP