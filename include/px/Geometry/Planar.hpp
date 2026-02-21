#ifndef PX_GEOMETRY_PLANE_HPP
#define PX_GEOMETRY_PLANE_HPP

#include "Axis.hpp"
#include "../Util/Fill.hpp"

namespace px
{
	namespace Geometry
	{
		class Planar
		{
		public:
			Planar(const std::vector<Vector3>& points, const Vector3& origin, const Vector3& normal)
				: points_(points.size()), origin_(origin), normal_(normal)
			{
				Vector3 rotationAxis = Vector::Unitise(Vector::Cross(normal, AxisZ()));
				float rotationAngle = Trigonometry::SafeArccosine(Vector::Dot(normal, AxisZ()));

				points_ = std::isnan(rotationAxis.x_) ?
					Fill<Vector2, Vector3>(points, [=](const Vector3& point3D) { return Vector::XY(Vector::Subtract(point3D, origin)); }) :
					Fill<Vector2, Vector3>(points, [=](const Vector3& point3D) { return Vector::XY(Vector::Rotate(Vector::Subtract(point3D, origin), rotationAxis, rotationAngle)); });
			}

			Planar(const std::vector<Vector2>& points, const Vector3& origin, const Vector3& normal)
				: points_(points), origin_(origin), normal_(normal)
			{
			}

			const std::vector<Vector2>& Get2D() const { return points_; }

			std::vector<Vector3> Get3D() const
			{
				Vector3 rotationAxis = Vector::Unitise(Vector::Cross(AxisZ(), normal_));
				float rotationAngle = Trigonometry::SafeArccosine(Vector::Dot(AxisZ(), normal_));

				return std::isnan(rotationAxis.x_) ?
					Fill<Vector3, Vector2>(points_, [=](const Vector2& point2D) { return Vector::Add(Vector::XYZ(point2D), origin_); }) :
					Fill<Vector3, Vector2>(points_, [=](const Vector2& point2D) { return Vector::Add(Vector::Rotate(Vector::XYZ(point2D), rotationAxis, rotationAngle), origin_); });
			}

			const Vector3& Origin() const { return origin_; }
			const Vector3& Normal() const { return normal_; }

		private:
			std::vector<Vector2> points_;
			Vector3 origin_;
			Vector3 normal_;
		};

	}
}

#endif // PX_GEOMETRY_PLANE_HPP