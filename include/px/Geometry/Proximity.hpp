#ifndef PX_GEOMETRY_PROXIMITY_HPP
#define PX_GEOMETRY_PROXIMITY_HPP

#include "Vector.hpp"
#include "Trigonometry.hpp"
#include "Mesh.hpp"

#include <vector>
#include <list>

namespace px
{
	namespace Geometry
	{
		namespace Proximity
		{
			//struct DistanceInformation
			//{
			//	DistanceInformation() : direction_(Vector::Zero<Vector3>()), distance_(0.0f) {}
			//	DistanceInformation(float initialDistance) : direction_(Vector::Zero<Vector3>()), distance_(initialDistance) {}
			//	DistanceInformation(const Vector3& direction, float distance) : direction_(direction), distance_(distance) {}

			//	Vector3 direction_;
			//	float distance_;
			//};

			//struct PointDistanceInformation
			//{
			//	PointDistanceInformation(unsigned int size) : distanceField_(size), pointID_(size, 0) {}

			//	std::vector<DistanceInformation> distanceField_;
			//	std::vector<unsigned int> pointID_;
			//};

			//struct PolylineDistanceInformation
			//{
			//	PolylineDistanceInformation(unsigned int size) : distanceField_(size), polylineID_(size), pointID_(size) {}

			//	std::vector<DistanceInformation> distanceField_;
			//	std::vector<unsigned int> polylineID_;
			//	std::vector<unsigned int> pointID_;
			//};

			//struct MeshDistanceInformation
			//{
			//	MeshDistanceInformation(unsigned int size) : distanceField_(size), triangleID_(size, 0) {}

			//	std::vector<DistanceInformation> distanceField_;
			//	std::vector<unsigned int> triangleID_;
			//};

			//std::vector<DistanceInformation> DistanceFieldSigned(const std::vector<Vector3>& field, const Vector3& planePoint, const Vector3& planeDirection);
			//PointDistanceInformation DistanceField(const std::vector<Vector3>& field, const std::vector<Vector3>& points);
			//PolylineDistanceInformation DistanceField(const std::vector<Vector3>& field, const std::vector<std::vector<Vector3>>& polylines);
			//MeshDistanceInformation DistanceField(const std::vector<Vector3>& field, const TriangleMesh2& mesh);
			//MeshDistanceInformation DistanceField(const std::vector<Vector3>& field, const TriangleMesh3& mesh);
			//MeshDistanceInformation DistanceFieldContained(const std::vector<Vector3>& field, const TriangleMesh3& mesh);			// use if all the field points are contained within mesh AABB bounding volume (optimised)
			//MeshDistanceInformation DistanceFieldSigned(const std::vector<Vector3>& field, const TriangleMesh2& mesh);
			//MeshDistanceInformation DistanceFieldSigned(const std::vector<Vector3>& field, const TriangleMesh3& mesh);
			//MeshDistanceInformation DistanceFieldContainedSigned(const std::vector<Vector3>& field, const TriangleMesh3& mesh);		// use if all the field points are contained within mesh AABB bounding volume (optimised)


			template <class T>
			static float PointPlaneDistance(const T& v, const T& p, const T& n)
			{
				return Vector::Dot(v - p, n);
			}

			struct Distances
			{
				std::vector<float> distances_;
				unsigned int indexMin_ = 0;
				unsigned int indexMax_ = 0;
			};

			template <class T>
			static Distances PointPlaneDistances(const std::vector<T>& points, const T& pP, const T& pN)
			{
				Distances result;

				result.distances_ = std::vector<float>(points.size(), 0);
				for (unsigned int p = 0; p < points.size(); ++p)
					result.distances_[p] = PointPlaneDistance<T>(points[p], pP, pN);

				// Find the indexes of the min and max distances...
				for (unsigned int d = 0; d < result.distances_.size(); ++d)
				{
					result.indexMax_ = result.distances_[d] > result.distances_[result.indexMax_] ? d : result.indexMax_;
					result.indexMin_ = result.distances_[d] < result.distances_[result.indexMin_] ? d : result.indexMin_;
				}

				return result;
			}

			template <class T>
			static Distances PointDistances(const std::vector<T>& points, const T& origin)
			{
				Distances result;

				result.distances_ = std::vector<float>(points.size(), 0);
				for (unsigned int p = 0; p < points.size(); ++p)
					result.distances_[p] = Vector::Length(points[p] - origin);

				// Find the indexes of the min and max distances...
				for (unsigned int d = 0; d < result.distances_.size(); ++d)
				{
					result.indexMax_ = result.distances_[d] > result.distances_[result.indexMax_] ? d : result.indexMax_;
					result.indexMin_ = result.distances_[d] < result.distances_[result.indexMin_] ? d : result.indexMin_;
				}

				return result;
			}


			template <class T>
			static int Direction(const T& v, const T& p, const T& n)
			{
				float d = PointPlaneDistance<T>(v, p, n);
				if (!d || std::isnan(d))
					return 0;
				return d > 0 ? 1 : -1;
			}

			template <class T>
			static int Direction(const T& v, const T& p, const T& n, float tolerance)
			{
				float d = PointPlaneDistance<T>(v, p, n);

				if (!d || std::isnan(d) || abs(d) < tolerance)
					return 0;
				return d > 0 ? 1 : -1;
			}

			template <class T>
			float RayPlaneDistance(const T& rP, const T& rD, const T& p, const T& n)
			{
				if (p == rP)
					return 0;

				float dotrDn = Vector::Dot(rD, n);
				return !dotrDn ? dotrDn : Vector::Dot(p - rP, n) / dotrDn;
			}

			//float SegmentPlaneDistance(const Vector3& l1, const Vector3& l2, const Vector3& p, const Vector3& n);
			//float SegmentPointDistance(const Vector3& l1, const Vector3& l2, const Vector3& p);

			template <class T>
			float LinePointDistance(const T& i, const T& j, const T& p)
			{
				// Vector rejection...
				T ip = p - i;
				float angle = Trigonometry::SafeArccosine(Vector::Dot(Vector::Unitise(j - i), Vector::Unitise(ip)));
				return Vector::Length(ip) * sin(angle);
			}

			// Segment Segment proximity
			template <class T>
			struct SegmentSegmentProximity
			{
				SegmentSegmentProximity() : distance_((std::numeric_limits<float>::max)()) {}
				SegmentSegmentProximity(const T& a, const T& b, float distance) : a_(a), b_(b), distance_(distance) {}

				T a_;
				T b_;
				float distance_;
			};

			//SegmentSegmentProximity<Vector2> SegmentSegment(const Vector2& a1, const Vector2& a2, const Vector2& b1, const Vector2& b2);
			//SegmentSegmentProximity<Vector2> PolylinePolyline(const std::vector<Vector2>& a, const std::vector<Vector2>& b);

			//// Polyline point proximity
			//template <class T>
			//struct ProximityResult
			//{
			//	ProximityResult() : distance_((std::numeric_limits<float>::max)()), polyline_(0) {}
			//	ProximityResult(const T& point, float distance) : point_(point), distance_(distance), polyline_(0) {}
			//	ProximityResult(const T& point, float distance, unsigned int polyline) : point_(point), distance_(distance), polyline_(polyline) {}

			//	T point_;
			//	float distance_;
			//	unsigned int polyline_;
			//};

			//ProximityResult<Vector2> PointPolyline(const std::vector<Vector2>& polyline, const Vector2& point);
			//ProximityResult<Vector2> PointPolylines(const std::list<const std::vector<Vector2>*>& polylines, const Vector2& point);

			//// Conformal layer...
			//class ConformalLayerInterface
			//{
			//public:
			//	virtual TriangleMesh3 Layer(float offset, float nudge) const = 0;
			//};

			//std::shared_ptr<ConformalLayerInterface> ConformalLayering(const TriangleMesh3& mesh, const AABB& extents, float voxelSize = 1.0f);
			//std::shared_ptr<ConformalLayerInterface> ConformalLayeringMeshes(const std::vector<AxW::Geometry::TriangleMesh3>& meshes, const AABB& extents, float voxelSize = 1.0f);
		}

	}
}


#endif // PX_GEOMETRY_PROXIMITY_HPP