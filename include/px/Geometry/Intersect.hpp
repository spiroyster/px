#ifndef PX_GEOMETRY_INTERSECTION_HPP
#define PX_GEOMETRY_INTERSECTION_HPP

#include "Vector.hpp"
#include "Proximity.hpp"
//#include "Triangle.hpp"
//#include "Polyline.hpp"

#include <optional>

namespace px
{
	namespace Geometry
	{
		namespace Intersect
		{

			// Plane intersections
			template <class T>
			static std::optional<T> LinePlane(const T& l1, const T& l2, const T& p, const T& n)
			{
				T rD = Vector::Unitise(l2 - l1);
				return l1 + (rD * Proximity::RayPlaneDistance<T>(l1, rD, p, n));
			}

			template <class T>
			static std::optional<T> RayPlane(const T& rP, const T& rD, const T& p, const T& n)
			{
				if (p == rP)
					return p;

				float dotrDn = Vector::Dot(rD, n);
				return !dotrDn ? std::optional<T>() : (rP + (rD * Vector::Dot(p - rP, n) / dotrDn));
			}

			/*template <class T>
			static std::optional<T> SegmentPlane(const T& i, const T& j, const T& p, const T& n)
			{
				if (auto intersectionPoint = LinePlane<T>(i, j, p, n))
					return Polyline::PointOnSegment<T>(i, j, *intersectionPoint) == Polyline::PointOnSegmentResult::NotOnSegment ? std::optional<T>() : *intersectionPoint;
				return std::optional<T>();
			}*/



			//// Triangle intersections
			//std::optional<Vector3> LineTriangle(const Vector3& l1, const Vector3& l2, const Vector3& a, const Vector3& b, const Vector3& c);
			//std::optional<Vector3> SegmentTriangle(const Vector3& i, const Vector3& j, const Vector3& a, const Vector3& b, const Vector3& c);
			//std::optional<Vector3> RayTriangle(const Vector3& rP, const Vector3& rD, const Vector3& a, const Vector3& b, const Vector3& c);

			//// 2D
			//std::optional<Vector2> RayLine(const Vector2& rP, const Vector2& rD, const Vector2& l1, const Vector2& l2);
			//std::optional<Vector2> RaySegment(const Vector2& rP, const Vector2& rD, const Vector2& i, const Vector2& j);
			//std::optional<Vector2> SegmentSegment(const Vector2& i1, const Vector2& j1, const Vector2& i2, const Vector2& j2);
			//static std::optional<Vector2> LineLine(const Vector2& a1, const Vector2& a2, const Vector2& b1, const Vector2& b2) { return RayLine(b1, Vector::Unitise(Vector::Subtract(b2, b1)), a1, a2); }


			//// Triangle Plane
			//class TrianglePlane
			//{
			//public:

			//	struct Segment
			//	{
			//		Segment(const Vector3& i, const Vector3& j) : i_(i), j_(j) {}
			//		Vector3 i_, j_, normal_;
			//	};

			//	struct Triangle
			//	{
			//		Triangle(const Vector3& a, const Vector3& b, const Vector3& c) : a_(a), b_(b), c_(c) {}
			//		Vector3 a_, b_, c_;
			//	};


			//	TrianglePlane(const Vector3& p, const Vector3& n, const Vector3& a, const Vector3& b, const Vector3& c, float clampToPlaneTolerance);
			//	TrianglePlane(const Vector3& p, const Vector3& n, const Vector3& a, const Vector3& b, const Vector3& c);

			//	bool IsCoplanar() const;
			//	bool IsPointOn() const;
			//	bool IsSinglePointOn() const;
			//	bool IsStraddling() const;
			//	bool EntirelyOutside() const;
			//	bool EntirelyInside() const;
			//	bool IsEdgeOn() const;
			//	bool ABMajor() const;
			//	bool BCMajor() const;
			//	bool CAMajor() const;
			//	bool Outside() const;
			//	bool Inside() const;
			//	bool Intersects() const;

			//	// This does not return if a triangle edge is on the plane, or if triangle is coplanar...
			//	std::optional<Segment> IntersectionEdge() const;

			//	// first is outside, second is inside list.. (this does not split coplanar triangles)
			//	std::pair<std::list<Triangle>, std::list<Triangle>> Split(bool coplanarIsInside);

			//	int AWeighting() const;
			//	int BWeighting() const;
			//	int CWeighting() const;

			//protected:
			//	TrianglePlane(const Vector3& p, const Vector3& n, const Vector3& a, const Vector3& b, const Vector3& c, bool);

			//	int aWeighting_, bWeighting_, cWeighting_;
			//	Vector3 a_, b_, c_, p_, n_;
			//};

			//// triangle A edges are the edges of triangle A that intersect B, and triangle B edges are vice versa...
			//struct TriangleTriangleIntersection
			//{
			//	std::list<TrianglePlane::Segment> triangleAEdges_, triangleBEdges_;
			//	bool isCoplanar_;
			//	bool Intersection() { return !triangleAEdges_.empty() || !triangleBEdges_.empty(); }
			//};

			//TriangleTriangleIntersection TriangleTriangle(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& aa, const Vector3& bb, const Vector3& cc);

			//// Segment intersections...
			//template <class T>
			//struct SegmentIntersections
			//{
			//	std::vector<T> points_;
			//	std::vector<std::vector<unsigned int>> indexes_;			// each segment has a list or zero or more intersection point indexes ...
			//};

			//// Calculate intersections between segments...
			//SegmentIntersections<Vector2> SegmentsSegments(const std::vector<Polyline::Segment*>& segments, const std::vector<Vector2>& vertices);

			//// Mesh intersections...
			//typedef std::pair<Vector3, unsigned int> RayMeshIntersection;

			//std::vector<RayMeshIntersection> RayMesh(const Vector3& rP, const Vector3& rD, const TriangleMesh3& mesh, unsigned int jitterCount = 0, float jitterRadius = 0.0f);
			//std::vector<std::optional<RayMeshIntersection>> RayMesh(const std::vector<Vector3>& rPositions, const std::vector<Vector3>& rDirections, const TriangleMesh3& mesh, unsigned int jitterCount = 0, float jitterRadius = 0.0f);
			//std::vector<unsigned int> MeshMesh(const TriangleMesh3& volume, const TriangleMesh3& mesh);

		}
	}
}

#endif // PX_GEOMETRY_INTERSECTION_HPP