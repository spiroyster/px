#ifndef PX_GEOMETRY_MATRIX_HPP
#define PX_GEOMETRY_MATRIX_HPP

#include "Vector.hpp"

namespace px
{
	namespace Geometry
	{
		class Matrix3;
		class Matrix4;

		// column major
		class Matrix2
		{
		public:
			Matrix2()
				: a_(0.0f), b_(0.0f), c_(0.0f), d_(0.0f)
			{
			}

			Matrix2(float a, float b, float c, float d)
				: a_(a), b_(b), c_(c), d_(d)
			{
			}

			bool operator==(const Matrix2& rhs) const
			{
				return a_ == rhs.a_ && b_ == rhs.b_
					&& c_ == rhs.c_ && d_ == rhs.d_;
			}

			float a_, b_,
				c_, d_;
		};

		class Matrix3
		{
		public:
			Matrix3()
				: a_(0.0f), b_(0.0f), c_(0.0f),
				d_(0.0f), e_(0.0f), f_(0.0f),
				g_(0.0f), h_(0.0f), i_(0.0f)
			{
			}

			Matrix3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
				: a_(a), b_(b), c_(c), d_(d), e_(e), f_(f),
				g_(g), h_(h), i_(i)
			{
			}

			bool operator==(const Matrix3& rhs) const
			{
				return a_ == rhs.a_ && b_ == rhs.b_ && c_ == rhs.c_
					&& d_ == rhs.d_ && e_ == rhs.e_ && f_ == rhs.f_
					&& g_ == rhs.g_ && h_ == rhs.h_ && i_ == rhs.i_;
			}

			float a_, b_, c_,
				d_, e_, f_,
				g_, h_, i_;
		};

		class Matrix4
		{
		public:

			Matrix4()
				: a_(0.0f), b_(0.0f), c_(0.0f), d_(0.0f),
				e_(0.0f), f_(0.0f), g_(0.0f), h_(0.0f),
				i_(0.0f), j_(0.0f), k_(0.0f), l_(0.0f),
				m_(0.0f), n_(0.0f), o_(0.0f), p_(0.0f)
			{
			}

			Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
				: a_(a), b_(b), c_(c), d_(d),
				e_(e), f_(f), g_(g), h_(h),
				i_(i), j_(j), k_(k), l_(l),
				m_(m), n_(n), o_(o), p_(p)
			{
			}

			bool operator==(const Matrix4& rhs) const
			{
				return a_ == rhs.a_ && b_ == rhs.b_ && c_ == rhs.c_ && d_ == rhs.d_
					&& e_ == rhs.e_ && f_ == rhs.f_ && g_ == rhs.g_ && h_ == rhs.h_
					&& i_ == rhs.i_ && j_ == rhs.j_ && k_ == rhs.k_ && l_ == rhs.l_
					&& m_ == rhs.m_ && n_ == rhs.n_ && o_ == rhs.o_ && p_ == rhs.p_;
			}

			float a_, b_, c_, d_,
				e_, f_, g_, h_,
				i_, j_, k_, l_,
				m_, n_, o_, p_;
		};

		namespace Matrix
		{
			// Add
			Matrix2 Add(const Matrix2& x, const Matrix2& y);
			Matrix3 Add(const Matrix3& x, const Matrix3& y);
			Matrix4 Add(const Matrix4& x, const Matrix4& y);

			// Subtract
			Matrix2 Subtract(const Matrix2& x, const Matrix2& y);
			Matrix3 Subtract(const Matrix3& x, const Matrix3& y);
			Matrix4 Subtract(const Matrix4& x, const Matrix4& y);

			// Product (matrix)
			Matrix2 Dot(const Matrix2& x, const Matrix2& y);
			Matrix3 Dot(const Matrix3& x, const Matrix3& y);
			Matrix4 Dot(const Matrix4& x, const Matrix4& y);

			// Hadamard Product (element-wise)
			Matrix2 Hadamard(const Matrix2& x, const Matrix2& y);
			Matrix3 Hadamard(const Matrix3& x, const Matrix3& y);
			Matrix4 Hadamard(const Matrix4& x, const Matrix4& y);

			// Transpose
			Matrix2 Transpose(const Matrix2& m);
			Matrix3 Transpose(const Matrix3& m);
			Matrix4 Transpose(const Matrix4& m);

			// Det
			float Determinant(const Matrix2& m);
			float Determinant(const Matrix3& m);
			float Determinant(const Matrix4& m);

			// Inverse
			Matrix2 Inverse(const Matrix2& m);
			Matrix3 Inverse(const Matrix3& m);
			Matrix4 Inverse(const Matrix4& m);

			// Sub  matricies...
			Matrix2 UpperMatrix2(const Matrix3& m);
			//Matrix2 UpperMatrix2(const Matrix4& m);
			Matrix3 UpperMatrix3(const Matrix4& m);

			template <class T>
			static T Zero() {}

			template <>
			static Matrix2 Zero<Matrix2>() { return Matrix2(); }

			template <>
			static Matrix3 Zero<Matrix3>() { return Matrix3(); }

			template <>
			static Matrix4 Zero<Matrix4>() { return Matrix4(); }

			template <class T>
			static T Identity() {}

			template <>
			static Matrix2 Identity<Matrix2>() { return Matrix2(1.0f, 0.0f, 0.0f, 1.0f); }

			template <>
			static Matrix3 Identity<Matrix3>() { return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

			template <>
			static Matrix4 Identity<Matrix4>() { return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

			// Translate 
			Matrix3 Translation(const Vector2& t);
			Matrix4 Translation(const Vector3& t);

			// Rotate
			Matrix3 Rotation(float angle);
			Matrix4 Rotation(const Vector3& axis, float angle);
			Matrix4 Rotation(const Vector3& fromVector, const Vector3& toVector);

			// Scale
			Matrix2 Scale(const Vector2& s);
			Matrix3 Scale(const Vector3& s);
			Matrix4 Scale(const Vector4& s);

			// Transform vector (non homogenous)
			Vector2 Transform(const Matrix2& m, const Vector2& v);
			Vector3 Transform(const Matrix3& m, const Vector3& v);
			Vector4 Transform(const Matrix4& m, const Vector4& v);

			// Transform vector (homogenous)
			Vector2 Transform(const Matrix3& m, const Vector2& v, float z);
			Vector3 Transform(const Matrix4& m, const Vector3& v, float w);

			//Homogenous Transformation Matrix Construction
			Matrix3 TransformationMatrix(const Matrix3& rotationMatrix, const Vector2& t);
			Matrix4 TransformationMatrix(const Matrix4& rotationMatrix, const Vector3& t);

			Vector3 EulerXYZ(const Matrix4& matrix);
			Matrix4 EulerXYZ(const Vector3& angles);
		}
	}
}

#endif // PX_GEOMETRY_MATRIX_HPP
