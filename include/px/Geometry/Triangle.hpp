#ifndef PX_GEOMETRY_TRIANGLE_HPP
#define PX_GEOMETRY_TRIANGLE_HPP

#include "Vector.hpp"

namespace px
{
	namespace Geometry
	{
		namespace Triangle
		{
			template <class T>
			static bool PointInTriangle(const T& a, const T& b, const T& c, const T& v) {}

			template <>
			static bool PointInTriangle<Vector2>(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& v)
			{
				return false;
			}

			template <>
			static bool PointInTriangle<Vector3>(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& v)
			{
				return (
					(Vector::Dot(Vector::Cross(Vector::Subtract(c, b), Vector::Subtract(v, b)), Vector::Cross(Vector::Subtract(c, b), Vector::Subtract(a, b))) >= 0) &&
					(Vector::Dot(Vector::Cross(Vector::Subtract(c, a), Vector::Subtract(v, a)), Vector::Cross(Vector::Subtract(c, a), Vector::Subtract(b, a))) >= 0) &&
					(Vector::Dot(Vector::Cross(Vector::Subtract(b, a), Vector::Subtract(v, a)), Vector::Cross(Vector::Subtract(b, a), Vector::Subtract(c, a))) >= 0)
					);
			}

			static Vector3 Normal(const Vector3& a, const Vector3& b, const Vector3& c)
			{
				return Vector::Unitise(Vector::Cross(Vector::Subtract(b, a), Vector::Subtract(c, a)));
			}

			template <class T>
			static T Centroid(const T& a, const T& b, const T& c)
			{
				return (a + b + c) / 3.0f;
			}


			/*template <class T>
			static T Incentre(const T& a, const T& b, const T& c)
			{

			}

			template <class T>
			static T Circumcentre(const T& a, const T& b, const T& c)
			{

			}*/

			template <class T>
			static float Area(const T& a, const T& b, const T& c)
			{
				T ab = b - a;
				float abMag = Vector::Length(ab);
				T ac = a - c;
				float acMag = Vector::Length(ac);

				// Check if points are colinear or abMag/acMag is big enough...
				if (abMag < 0.0001f || acMag < 0.0001f)
					return 0.0f;

				float cosThita = Vector::Dot(Vector::Unitise(ab), Vector::Unitise(ac));
				float area = 0.5f * abMag * acMag * sqrt(1.0f - (cosThita * cosThita));
				return std::isnan(area) ? 0.0f : area;
			}

			/*template <class T>
			static std::list<T> SegmentClip(const T& a, const T& b, const T& c, const T& i, const T& j, float tolerance)
			{

			}*/

			//struct TriangleSplitSegment
			//{
			//	TriangleSplitSegment(const std::vector<AxW::Geometry::Intersect::TrianglePlane::Segment>& inside,
			//		const std::vector<AxW::Geometry::Intersect::TrianglePlane::Segment>& outside) : inside_(inside), outside_(outside)
			//	{
			//	}

			//	std::vector<AxW::Geometry::Intersect::TrianglePlane::Segment> inside_;
			//	std::vector<AxW::Geometry::Intersect::TrianglePlane::Segment> outside_;
			//};


			//TriangleSplitSegment SegmentClip(const AxW::Geometry::Vector3& i, const AxW::Geometry::Vector3& j,
			//	const AxW::Geometry::Vector3& a, const AxW::Geometry::Vector3& b, const AxW::Geometry::Vector3& c)
			//{
			//	// Calculate the tangent normals of the triangle edges...
			//	AxW::Geometry::Vector3 normal = Normal(a, b, c);
			//	//AxW::Geometry::Vector3D abN = AxW::Geometry::Vector::Cross(AxW::Geometry::Vector::Unitise(b - a), normal);

			//	// We need to find the portion of segment that is inside...

			//	// First check ab edge...
			//	SegmentSplit segmentSplit = SegmentSplitSegment(i, j, a, AxW::Geometry::Vector::Cross(AxW::Geometry::Vector::Unitise(b - a), normal));

			//	if (!segmentSplit.inside_.empty())
			//	{
			//		// Then check bc edge...
			//		segmentSplit = SegmentSplitSegment(segmentSplit.inside_.front(), segmentSplit.inside_.back(), b, AxW::Geometry::Vector::Cross(AxW::Geometry::Vector::Unitise(c - b), normal));

			//		if (!segmentSplit.inside_.empty())
			//		{
			//			// Then check ca edge...
			//			segmentSplit = SegmentSplitSegment(segmentSplit.inside_.front(), segmentSplit.inside_.back(), c, AxW::Geometry::Vector::Cross(AxW::Geometry::Vector::Unitise(a - c), normal));
			//		}
			//	}

			//	if (segmentSplit.inside_.empty())
			//		return TriangleSplitSegment({}, { AxW::Geometry::Intersect::TrianglePlane::Segment(i, j) });

			//	float insideIToI = AxW::Geometry::Vector::MagnitudeSquared(segmentSplit.inside_.front() - i);
			//	float insideJToI = AxW::Geometry::Vector::MagnitudeSquared(segmentSplit.inside_.back() - i);

			//	if (insideIToI < insideJToI)
			//	{
			//		return TriangleSplitSegment(
			//			{ AxW::Geometry::Intersect::TrianglePlane::Segment(segmentSplit.inside_.front(), segmentSplit.inside_.back()) },
			//			{ AxW::Geometry::Intersect::TrianglePlane::Segment(i, segmentSplit.inside_.front()),
			//			  AxW::Geometry::Intersect::TrianglePlane::Segment(segmentSplit.inside_.back(), j) }
			//		);
			//	}
			//	else
			//	{
			//		return TriangleSplitSegment(
			//			{ AxW::Geometry::Intersect::TrianglePlane::Segment(segmentSplit.inside_.front(), segmentSplit.inside_.back()) },
			//			{ AxW::Geometry::Intersect::TrianglePlane::Segment(i, segmentSplit.inside_.back()),
			//			  AxW::Geometry::Intersect::TrianglePlane::Segment(segmentSplit.inside_.front(), j) }
			//		);
			//	}

			//}



		}
	}
}

#endif // PX_GEOMETRY_TRIANGLE_HPP