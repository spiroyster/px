#ifndef PX_GEOMETRY_AXIS_HPP
#define PX_GEOMETRY_AXIS_HPP

#include "Matrix.hpp"
#include "Trigonometry.hpp"
#include "../Util/Fill.hpp"

namespace px
{
	namespace Geometry
	{
		/// <summary>
		/// The X axis in 3D coordinate space.
		/// </summary>
		static Vector3 AxisX() { return { 1.0f, 0, 0 }; }

		/// <summary>
		/// The Y axis in 3D coordinate space.
		/// </summary>
		static Vector3 AxisY() { return { 0, 1.0f, 0 }; }

		/// <summary>
		/// The Z axis in 3D coordinate space.
		/// </summary>
		static Vector3 AxisZ() { return { 0, 0, 1.0f }; }

		/// <summary>
		/// The Negative X axis in 3D coordinate space.
		/// </summary>
		static Vector3 AxisNegativeX() { return { -1.0f, 0, 0 }; }

		/// <summary>
		/// The Negative Y axis in 3D coordinate space.
		/// </summary>
		static Vector3 AxisNegativeY() { return { 0, -1.0f, 0 }; }

		/// <summary>
		/// The Negative Z axis in 3D coordinate space.
		/// </summary>
		static Vector3 AxisNegativeZ() { return { 0, 0, -1.0f }; }

		/// <summary>
		/// Helper object representing an orthogonal axis. This enforces orthogonality when manipulating any of the axis.
		/// </summary>
		class Axis
		{
		public:

			/// <summary>
			/// Default construction of axis i(1,0,0), j(0,1,0) and k(0,0,1)... (aka x,y,z axis).
			/// </summary>
			Axis() : i_(AxisX()), j_(AxisY()), k_(AxisZ()) {}

			/// <summary>
			/// Constrcut from a single 'k' axis. i,j axis are constructed using LH coordinate system.
			/// </summary>
			/// <param name="k">The k axis to construct the other orthogonal axis from.</param>
			Axis(const Vector3& k)
			{
				Axis defaultAxis;
				k_ = Vector::Unitise(k);

				if (Vector::Equals(k_, defaultAxis.J(), EqualsTolerance))
					i_ = Vector::Cross(defaultAxis.K(), k_);
				else if (Vector::Equals(k_, Vector::Reverse(defaultAxis.J()), EqualsTolerance))
					i_ = Vector::Cross(Vector::Reverse(defaultAxis.K()), k_);
				else
					i_ = Vector::Cross(defaultAxis.J(), k_);

				j_ = Vector::Cross(k_, i_);

				i_ = Vector::Unitise(i_);
				j_ = Vector::Unitise(j_);
			}

			/// <summary>
			/// Construct an arbitary orthogonal axis. Caller is repsonsible for ensuring orthogonality of i,j,k vectors when constructing.
			/// </summary>
			/// <param name="i">The i axis</param>
			/// <param name="j">The j axis</param>
			/// <param name="k">The k axis</param>
			Axis(const Vector3& i, const Vector3& j, const Vector3& k)
				: i_(i), j_(j), k_(k)
			{
				// Check axis are orthogonal???
			}

			/// <summary>
			/// The I (along) axis of this orthogonal axis.
			/// </summary>
			/// <returns>The i axis</returns>
			const Vector3& I() const { return i_; }

			/// <summary>
			/// The J axis (up) of this orthogonal axis.
			/// </summary>
			/// <returns>The j axis</returns>
			const Vector3& J() const { return j_; }

			/// <summary>
			/// The K (in) axis of this orthogonal axis.
			/// </summary>
			/// <returns>The k axis</returns>
			const Vector3& K() const { return k_; }

			/// <summary>
			/// Rotate this orthogonal axis about an arbitary axis and angle.
			/// </summary>
			/// <param name="radians">The rotation angle in raidans</param>
			/// <param name="axis">The rotation axis</param>
			void Rotate(float radians, const Vector3& axis)
			{
				i_ = Vector::Unitise(Vector::Rotate(i_, axis, radians));
				j_ = Vector::Unitise(Vector::Rotate(j_, axis, radians));
				k_ = Vector::Unitise(Vector::Rotate(k_, axis, radians));
			}

			/// <summary>
			/// Transform this orthogonal axis.
			/// </summary>
			/// <param name="transform">The transformation matrix to transform this axis</param>
			void Transform(const Matrix4& transform)
			{
				i_ = Vector::Unitise(Matrix::Transform(transform, i_, 0.0f));
				j_ = Vector::Unitise(Matrix::Transform(transform, j_, 0.0f));
				k_ = Vector::Unitise(Matrix::Transform(transform, k_, 0.0f));
			}

			/// <summary>
			/// Calculate a transformation matrix to transform this axis system to the supplied reference system.
			/// </summary>
			/// <param name="reference">The reference axis to use as basis</param>
			/// <returns>The transformation matrix from reference to this axis</returns>
			Matrix4 Basis(const Axis& to) const
			{
				Matrix4 transform = Matrix::Identity<Matrix4>();

				// Construct a working axis to transform...
				Axis workingAxis = *this;

				// Perform the first rotation to align the i axis...
				Vector3 rotationAxis = Vector::Unitise(Vector::Cross(i_, to.I()));
				float rotationAngle = Trigonometry::SafeArccosine(Vector::Dot(i_, to.I()));

				// If rotation is valid, transform to align the i axis...
				if (!std::isnan(rotationAxis.x_) && rotationAngle)
				{
					// Create our rotation matrix...
					transform = Matrix::Rotation(rotationAxis, rotationAngle);

					// Apply this rotation to our workingAxis
					workingAxis.Rotate(rotationAngle, rotationAxis);
				}
				// Check for gimbal lock..
				else if (Vector::Magnitude(Vector::Subtract(i_, to.I())) > 1.0f)
				{
					rotationAxis = k_;
					rotationAngle = Trigonometry::Pi::f;

					// Apply rotation to both transform and working axis...
					transform = Matrix::Rotation(rotationAxis, rotationAngle);
					workingAxis.Rotate(rotationAngle, rotationAxis);
				}

				// Use the working axis to perform the second rotation which corrects the skew...
				rotationAxis = Vector::Unitise(Vector::Cross(workingAxis.J(), to.J()));
				rotationAngle = Trigonometry::SafeArccosine(Vector::Dot(workingAxis.J(), to.J()));

				if (!std::isnan(rotationAxis.x_) && rotationAngle)
					transform = Matrix::Rotation(rotationAxis, rotationAngle) * transform;

				return transform;
			}

		protected:
			Vector3 i_, j_, k_;
		};
	}
}

#endif // PX_GEOMETRY_AXIS_HPP