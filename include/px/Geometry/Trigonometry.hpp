#ifndef PX_GEOMETRY_TRIGONOMETRY_HPP
#define PX_GEOMETRY_TRIGONOMETRY_HPP

#include <cmath>

namespace px
{
	namespace Geometry
	{
		namespace Trigonometry
		{
			/// <summary>
			/// Various representations of PI.
			/// </summary>
			namespace Pi
			{
				/// <summary>
				/// Pi as float.
				/// </summary>
				constexpr const float f = 3.14159265358979f;

				/// <summary>
				/// Pi as double.
				/// </summary>
				constexpr const double d = 3.141592653589793238;

				/// <summary>
				/// Half Pi as 
				/// </summary>
				constexpr const float HalfPi = Pi::f * 0.5f;

				/// <summary>
				/// Two Pi as float.
				/// </summary>
				constexpr const float TwoPi = Pi::f * 2.0f;
			}

			/// <summary>
			/// Degrees to Radians scaler.
			/// </summary>
			constexpr const float DegToRad = Pi::f / 180.0f;

			/// <summary>
			/// Radians to Degrees scaler.
			/// </summary>
			constexpr const float RadToDeg = 180.0f / Pi::f;

			/// <summary>
			/// Safe acos. Since maths.h acos function fails if value < -1.0 || > 1.0, this function clamps before calling acos.
			/// </summary>
			/// <param name="val">Value to perform safe acos on.</param>
			/// <returns>Arcosine of value</returns>
			static float SafeArccosine(float cosThita)
			{
				if (cosThita <= -1.0f)
					return Pi::f;
				if (cosThita >= 1.0f)
					return 0;
				return acos(cosThita);
			}

			/// <summary>
			/// Convert radians to degrees.
			/// </summary>
			/// <param name="rad">The value in radians to convert.</param>
			/// <returns>The value in degrees</returns>
			static float RadiansToDegrees(float rad) { return rad * RadToDeg; }

			/// <summary>
			/// Convert degrees to radians.
			/// </summary>
			/// <param name="deg">The value in degrees to convert.</param>
			/// <returns>The value in radians</returns>
			static float DegreesToRadians(float deg) { return deg * DegToRad; }

			// Cyclic clamp (i.e 0 < phi < 2Pi) or (-pi < phi < pi)
			//float ClampZero2Pi(float phi);
			//float ClampNegavtivePiPi(float phi);

			// Wrap to 0 < phi < 2pi
			static float WrapZero2Pi(float phi)
			{
				return phi - Pi::TwoPi * floor(phi / Pi::TwoPi);
				//return angle < 0 ? AxW::Geometry::Trigonometry::PI::TwoPi + angle : angle;
			}

			// Wrap to -pi < phi < pi
			static float WrapPi(float phi)
			{
				return WrapZero2Pi(phi) - Pi::f;
			}
		}
	}
}

#endif // PX_GEOMETRY_TRIGONOMETRY_HPP