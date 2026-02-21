#include <include/px/Geometry/Vector.hpp>
#include <include/px/Geometry/Trigonometry.hpp>

#include <cmath>
#include <vector>

namespace px
{
	namespace Geometry
	{
		namespace Vector
		{
			bool Equals(const Vector2& a, const Vector2& b, float e)
			{
				return Magnitude(Subtract(b, a)) < e;
			}
			bool Equals(const Vector3& a, const Vector3& b, float e)
			{
				return Magnitude(Subtract(b, a)) < e;
			}
			bool Equals(const Vector4& a, const Vector4& b, float e)
			{
				return Magnitude(Subtract(b, a)) < e;
			}
			Vector2 Add(const Vector2& a, const Vector2& b)
			{
				return
				{
					a.x_ + b.x_,
					a.y_ + b.y_
				};
			}
			Vector3 Add(const Vector3& a, const Vector3& b)
			{
				return
				{
					a.x_ + b.x_,
					a.y_ + b.y_,
					a.z_ + b.z_
				};
			}
			Vector4 Add(const Vector4& a, const Vector4& b)
			{
				return
				{
					a.x_ + b.x_,
					a.y_ + b.y_,
					a.z_ + b.z_,
					a.w_ + b.w_
				};
			}
			Vector2 Subtract(const Vector2& a, const Vector2& b)
			{
				return
				{
					a.x_ + b.x_,
					a.y_ + b.y_
				};
			}
			Vector3 Subtract(const Vector3& a, const Vector3& b)
			{
				return
				{
					a.x_ + b.x_,
					a.y_ + b.y_,
					a.z_ + b.z_
				};
			}
			Vector4 Subtract(const Vector4& a, const Vector4& b)
			{
				return
				{
					a.x_ + b.x_,
					a.y_ + b.y_,
					a.z_ + b.z_,
					a.w_ + b.w_
				};
			}
			Vector2 Scale(const Vector2& a, float s)
			{
				return
				{
					a.x_ * s,
					a.y_ * s
				};
			}
			Vector3 Scale(const Vector3& a, float s)
			{
				return
				{
					a.x_ * s,
					a.y_ * s,
					a.z_ * s
				};
			}
			Vector4 Scale(const Vector4& a, float s)
			{
				return
				{
					a.x_ * s,
					a.y_ * s,
					a.z_ * s,
					a.w_ * s
				};
			}
			Vector2 Divide(const Vector2& a, float s)
			{
				return
				{
					a.x_ / s,
					a.y_ / s
				};
			}
			Vector3 Divide(const Vector3& a, float s)
			{
				return
				{
					a.x_ / s,
					a.y_ / s,
					a.z_ / s
				};
			}
			Vector4 Divide(const Vector4& a, float s)
			{
				return
				{
					a.x_ / s,
					a.y_ / s,
					a.z_ / s,
					a.w_ / s
				};
			}
			float Dot(const Vector2& a, const Vector2& b)
			{
				return glm::dot(GLM(a), GLM(b));
			}
			float Dot(const Vector3& a, const Vector3& b)
			{
				return glm::dot(GLM(a), GLM(b));
			}
			float Dot(const Vector4& a, const Vector4& b)
			{
				return glm::dot(GLM(a), GLM(b));
			}
			Vector3 Cross(const Vector3& a, const Vector3& b)
			{
				return GVector3(glm::cross(GLM(a), GLM(b)));
			}
			Vector2 Unitise(const Vector2& v)
			{
				float m = Magnitude(v);
				return
				{
					v.x_ / m,
					v.y_ / m
				};
			}
			Vector3 Unitise(const Vector3& v)
			{
				float m = Magnitude(v);
				return
				{
					v.x_ / m,
					v.y_ / m,
					v.z_ / m
				};
			}
			Vector4 Unitise(const Vector4& v)
			{
				float m = Magnitude(v);
				return
				{
					v.x_ / m,
					v.y_ / m,
					v.z_ / m,
					v.w_ / m
				};
			}
			float Magnitude(const Vector2& v)
			{
				return std::sqrt((v.x_ * v.x_) + (v.y_ * v.y_));
			}
			float Magnitude(const Vector3& v)
			{
				return std::sqrt((v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_));
			}
			float Magnitude(const Vector4& v)
			{
				return std::sqrt((v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_) + (v.w_ * v.w_));
			}
			float MagnitudeSquared(const Vector2& v)
			{
				return (v.x_ * v.x_) + (v.y_ * v.y_);
			}
			float MagnitudeSquared(const Vector3& v)
			{
				return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_);
			}
			float MagnitudeSquared(const Vector4& v)
			{
				return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_) + (v.w_ * v.w_);
			}
			Vector2 Reverse(const Vector2& v)
			{
				return { -v.x_, -v.y_ };
			}
			Vector3 Reverse(const Vector3& v)
			{
				return { -v.x_, -v.y_, -v.z_ };
			}
			Vector4 Reverse(const Vector4& v)
			{
				return { -v.x_, -v.y_, -v.z_, -v.w_ };
			}
			Vector2 Perpindicular(const Vector2& v, bool ccw)
			{
				return ccw ? Vector2{ -v.y_, v.x_ } : Vector2{ v.y_, -v.x_ };
			}
			Vector3 Perpindicular(const Vector3& v, const Vector3& axis, bool ccw)
			{
				return ccw ? Cross(axis, v) : Cross(v, axis);
			}
			float Angle(const Vector2& i, const Vector2& j)
			{
				return std::atan2(i.x_ * j.y_ - i.y_ * j.x_, Dot(i, j));
			}
			float Angle(const Vector3& i, const Vector3& j)
			{
				float crossX = i.y_ * j.z_ - i.z_ * j.y_;
				float crossY = i.z_ * j.x_ - i.x_ * j.z_;
				float crossZ = i.x_ * j.y_ - i.y_ * j.x_;
				float det = std::sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
				return std::atan2(det, Dot(i, j));
			}
			float Angle(const Vector3& i, const Vector3& j, const Vector3& axis)
			{
				float angle = Trigonometry::SafeArccosine(Dot(i, j));
				if (angle)
				{
					float diff = Magnitude(Subtract(Unitise(Cross(i, j)), axis));
					if (diff > 1.5f)
						angle = -angle;
				}
				return angle;
			}

			Vector2 Rotate(const Vector2& v, float thita)
			{
				float cosThita = std::cos(thita);
				float sinThita = std::sin(thita);
				return { (v.x_ * cosThita) - (v.y_ * sinThita), (v.x_ * sinThita) + (v.y_ * cosThita) };
			}

			Vector3 Rotate(const Vector3& v, const Vector3& axis, float angle)
			{
				float c = static_cast<float>(std::cos(angle));
				Vector3 vRot = v * c;
				vRot = Cross(axis, v) * static_cast<float>(std::sin(angle)) + vRot;
				vRot = axis * (Dot(axis, v) * (1.0f - c)) + vRot;
				return vRot;
			}

			Planar::Planar(const std::vector<Vector3>& points, const Vector3& origin, const Vector3& normal)
				: points_(points.size()), origin_(origin), normal_(normal)
			{
				Vector3 rotationAxis = Unitise(Cross(normal, AxisZ()));
				float rotationAngle = Trigonometry::SafeArccosine(Dot(normal, AxisZ()));

				points_ = std::isnan(rotationAxis.x_) ?
					Fill<Vector2, Vector3>(points, [=](const Vector3& point3D) { return XY(point3D - origin); }) :
					Fill<Vector2, Vector3>(points, [=](const Vector3& point3D) { return XY(Rotate(point3D - origin, rotationAxis, rotationAngle)); });
			}

			Planar::Planar(const std::vector<Vector2>& points, const Vector3& origin, const Vector3& normal)
				: points_(points), origin_(origin), normal_(normal)
			{
			}

			const std::vector<Vector2>& Planar::Get2D() const { return points_; }

			std::vector<Vector3> Planar::Get3D() const
			{
				Vector3 rotationAxis = Unitise(Cross(AxisZ(), normal_));
				float rotationAngle = Trigonometry::SafeArccosine(Dot(AxisZ(), normal_));

				return std::isnan(rotationAxis.x_) ?
					Fill<Vector3, Vector2>(points_, [=](const Vector2& point2D) { return Add(XYZ(point2D), origin_); }) :
					Fill<Vector3, Vector2>(points_, [=](const Vector2& point2D) { return Add(Rotate(XYZ(point2D), rotationAxis, rotationAngle), origin_); });
			}

			Vector2 Round(const Vector2& v, float precision)
			{
				float x = std::round(v.x_ * precision) / precision;
				float y = std::round(v.y_ * precision) / precision;
				return { x, y };
			}

			Vector3 Round(const Vector3& v, float precision)
			{
				float x = std::round(v.x_ * precision) / precision;
				float y = std::round(v.y_ * precision) / precision;
				float z = std::round(v.z_ * precision) / precision;
				return { x, y, z };
			}
		}
	}
}