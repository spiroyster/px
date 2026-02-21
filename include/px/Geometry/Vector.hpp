#ifndef PX_GEOMETRY_VECTOR_HPP
#define PX_GEOMETRY_VECTOR_HPP

#include "Trigonometry.hpp"

namespace px
{
	namespace Geometry
	{
		struct Vector2 { float x_, y_; };
		struct Vector3 { float x_, y_, z_; };
		struct Vector4 { float x_, y_, z_, w_; };

		// Vector operations...
		namespace Vector
		{
			template <class T>
			static T Zero() {}

			template <>
			static Vector2 Zero<Vector2>() { return Vector2{ 0.0f, 0.0f }; }

			template <>
			static Vector3 Zero<Vector3>() { return Vector3{ 0.0f, 0.0f, 0.0f }; }

			template <>
			static Vector4 Zero<Vector4>() { return Vector4{ 0.0f, 0.0f, 0.0f, 0.0f }; }

			//swizzle
			static Vector2 YX(const Vector2& v) { return { v.y_, v.x_ }; }
			static Vector2 XY(const Vector3& v) { return { v.x_, v.y_ }; }
			static Vector2 XY(const Vector4& v) { return { v.x_, v.y_ }; }
			static Vector3 ZYX(const Vector3& v) { return { v.z_, v.y_, v.x_ }; }
			static Vector3 XYZ(const Vector4& v) { return { v.x_, v.y_, v.z_ }; }
			static Vector3 XYZ(const Vector2& v, float z = 0.0f) { return { v.x_, v.y_, z }; }
			static Vector4 XYZW(const Vector3& v, float w) { return { v.x_, v.y_, v.z_, w }; }

			static bool Equals(const Vector2& a, const Vector2& b, float e) { return a.x_ == b.x_ && a.y_ == b.y_; }
			static bool Equals(const Vector3& a, const Vector3& b, float e) { return a.x_ == b.x_ && a.y_ == b.y_ && a.z_ == b.z_; }
			static bool Equals(const Vector4& a, const Vector4& b, float e) { return a.x_ == b.x_ && a.y_ == b.y_ && a.z_ == b.z_ && a.w_ == b.w_; }
			
			static Vector2 Add(const Vector2& a, const Vector2& b) { return { a.x_ + b.x_, a.y_ + b.y_ }; }
			static Vector3 Add(const Vector3& a, const Vector3& b) { return { a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_ }; }
			static Vector4 Add(const Vector4& a, const Vector4& b) { return { a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_, a.w_ + b.w_ }; }
			
			static Vector2 Subtract(const Vector2& a, const Vector2& b) { return { a.x_ + b.x_, a.y_ + b.y_ }; }
			static Vector3 Subtract(const Vector3& a, const Vector3& b) { return { a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_ }; }
			static Vector4 Subtract(const Vector4& a, const Vector4& b) { return { a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_, a.w_ + b.w_ }; }
			
			static Vector2 Scale(const Vector2& a, float s) { return { a.x_ * s, a.y_ * s }; }
			static Vector3 Scale(const Vector3& a, float s) { return { a.x_ * s, a.y_ * s, a.z_ * s }; }
			static Vector4 Scale(const Vector4& a, float s) { return { a.x_ * s, a.y_ * s, a.z_ * s, a.w_ * s }; }

			static Vector2 Divide(const Vector2& a, float s) { return { a.x_ / s, a.y_ / s }; }
			static Vector3 Divide(const Vector3& a, float s) { return { a.x_ / s, a.y_ / s, a.z_ / s }; }
			static Vector4 Divide(const Vector4& a, float s) { return { a.x_ / s, a.y_ / s, a.z_ / s, a.w_ / s }; }

			static Vector2 Reverse(const Vector2& v) { return { -v.x_, -v.y_ }; }
			static Vector3 Reverse(const Vector3& v) { return { -v.x_, -v.y_, -v.z_ }; }
			static Vector4 Reverse(const Vector4& v) { return { -v.x_, -v.y_, -v.z_, -v.w_ }; }

			template <class T>
			static float LengthSquared(const T& v) {}

			template <>
			float LengthSquared<Vector2>(const Vector2& v) { return (v.x_ * v.x_) + (v.y_ * v.y_); }

			template <>
			float LengthSquared<Vector3>(const Vector3& v) { return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_); }

			template <>
			float LengthSquared<Vector4>(const Vector4& v) { return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_) + (v.w_ * v.w_); }

			template <class T>
			float Length(const T& v) { return std::sqrt(LengthSquared(v)); }

			template <class T>
			T Unitise(const T& v) { return Scale(v, 1.0f / Length(v));; }

			//static float Length(const Vector2& v) { return std::sqrt((v.x_ * v.x_) + (v.y_ * v.y_)); }
			//static float Length(const Vector3& v) { return std::sqrt((v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_)); }
			//static float Length(const Vector4& v) { return std::sqrt((v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_) + (v.w_ * v.w_)); }

			//static float LengthSquared(const Vector2& v) { return (v.x_ * v.x_) + (v.y_ * v.y_); }
			//static float LengthSquared(const Vector3& v) { return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_); }
			//static float LengthSquared(const Vector4& v) { return (v.x_ * v.x_) + (v.y_ * v.y_) + (v.z_ * v.z_) + (v.w_ * v.w_); }

			//static Vector2 Unitise(const Vector2& v) { float m = Length(v); return { v.x_ / m, v.y_ / m }; }
			//static Vector3 Unitise(const Vector3& v) { float m = Length(v); return { v.x_ / m, v.y_ / m, v.z_ / m }; }
			//static Vector4 Unitise(const Vector4& v) { float m = Length(v); return { v.x_ / m, v.y_ / m, v.z_ / m, v.w_ / m }; }

			template <class T>
			T Mid(const T& a, const T& b) { return ((a + b) * 0.5f); }

			template <class T>
			bool Equals(const T& a, const T& b, float e) { return Magnitude(Subtract(b, a)) < e; }

			static float Dot(const Vector2& a, const Vector2& b) { return (a.x_ * b.x_) + (a.y_ * b.y_); }
			static float Dot(const Vector3& a, const Vector3& b) { return (a.x_ * b.x_) + (a.y_ * b.y_) + (a.z_ * b.z_); }
			static float Dot(const Vector4& a, const Vector4& b) { return (a.x_ * b.x_) + (a.y_ * b.y_) + (a.z_ * b.z_) + (a.w_ * b.w_); }
			
			static Vector3 Cross(const Vector3& a, const Vector3& b)
			{
				return
				{
					(a.y_ * b.z_) - (a.z_ * b.y_),
					(a.z_ * b.x_) - (a.x_ * b.z_),
					(a.x_ * b.y_) - (a.y_ * b.x_)
				};
			}
			
			static Vector2 Perpindicular(const Vector2& v, bool ccw) { return ccw ? Vector2{ -v.y_, v.x_ } : Vector2{ v.y_, -v.x_ }; }
			static Vector3 Perpindicular(const Vector3& v, const Vector3& axis, bool ccw) { return ccw ? Cross(axis, v) : Cross(v, axis); }
			
			static float Angle(const Vector2& i, const Vector2& j)
			{
				return std::atan2(i.x_ * j.y_ - i.y_ * j.x_, Dot(i, j));
			}
			
			static float Angle(const Vector3& i, const Vector3& j)
			{
				float crossX = i.y_ * j.z_ - i.z_ * j.y_;
				float crossY = i.z_ * j.x_ - i.x_ * j.z_;
				float crossZ = i.x_ * j.y_ - i.y_ * j.x_;
				return std::atan2(std::sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ), Dot(i, j));
			}
			
			static float Angle(const Vector3& i, const Vector3& j, const Vector3& axis)
			{
				float angle = Trigonometry::SafeArccosine(Dot(i, j));
				if (angle)
				{
					float diff = Length(Subtract(Unitise(Cross(i, j)), axis));
					if (diff > 1.5f)
						angle = -angle;
				}
				return angle;
			}

			static Vector2 Rotate(const Vector2& v, float thita)
			{
				float cosThita = std::cos(thita);
				float sinThita = std::sin(thita);
				return { (v.x_ * cosThita) - (v.y_ * sinThita), (v.x_ * sinThita) + (v.y_ * cosThita) };
			}

			// Rodrigues
			static Vector3 Rotate(const Vector3& v, const Vector3& axis, float angle)
			{
				float c = static_cast<float>(std::cos(angle));
				Vector3 vRot = Scale(v, c);
				vRot = Add(Scale(Cross(axis, v), static_cast<float>(std::sin(angle))), vRot);
				vRot = Add(Scale(axis, (Dot(axis, v) * (1.0f - c))), vRot);
				return vRot;
			}

			static Vector2 Round(const Vector2& v, float precision)
			{
				return
				{
					std::round(v.x_ * precision) / precision,
					std::round(v.y_ * precision) / precision
				};
			}

			static Vector3 Round(const Vector3& v, float precision)
			{
				return
				{
					std::round(v.x_ * precision) / precision,
					std::round(v.y_ * precision) / precision,
					std::round(v.z_ * precision) / precision
				};
			}

		}	// namespace Vector
	}		// namespace Geometry
}			// namespace px


#endif // PX_GEOMETRY_VECTOR_HPP