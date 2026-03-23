#ifndef PX_GEOMETRY_POLYLINE_HPP
#define PX_GEOMETRY_POLYLINE_HPP

#include "VertexSet.hpp"
#include "Triangle.hpp"

#include <list>
#include <vector>

namespace px
{
	namespace Geometry
	{
		namespace Polyline
		{
			// Segment
			struct Segment
			{
				Segment(unsigned int i, unsigned int j) : i_(i), j_(j) {}
				virtual ~Segment() {}

				unsigned int i_, j_;
			};

			// Continuous
			std::list<std::list<Segment*>> Continuous(const std::list<Segment*>& segments);

			// Unique vertex list
			std::vector<unsigned int> UniqueIndexes(const std::list<Segment*>& segments);

			// Point on segment
			enum PointOnSegmentResult
			{
				NotOnSegment = 0,
				EqualVertexI,
				EqualVertexJ,
				OnLine
			};

			template <class T>
			PointOnSegmentResult PointOnSegment(const T& i, const T& j, const T& point, float tolerance)
			{
				if (Vector::Equals(i, point, tolerance))
					return PointOnSegmentResult::EqualVertexI;
				if (Vector::Equals(j, point, tolerance))
					return PointOnSegmentResult::EqualVertexJ;
				return abs(Vector::Length(point - i) + Vector::Length(point - j) - Vector::Length(j - i)) < tolerance ? PointOnSegmentResult::OnLine : PointOnSegmentResult::NotOnSegment;
			}


			// Normals
			template <class T>
			struct PolylineNormalsResult
			{
				std::vector<T> segmentNormals_;
				std::vector<T> vertexNormals_;
			};

			// Assumed CCW winding 
			// N.B for open - vertexNormals = number of points, segmentNormals = (number of points - 1)... last/first vertex normal perpindicular to segment
			//     for closed - vertexNormals = number of points, segmentNormals = number of points... first vertex normal normalized (prev segment + next segment)
			PolylineNormalsResult<Vector2> Normals(const std::vector<Vector2>& points, bool close);
			PolylineNormalsResult<Vector3> Normals(const std::vector<Vector3>& points, bool close);

			//// Assumed closed (front != back)...
			//std::vector<Vector2> SegmentNormals(const std::vector<Vector2>& loop)
			//{
			//	std::vector<AxW::Geometry::Vector2> result(isLoop ? polyline.size() : polyline.size() - 1);

			//	for (unsigned int p = 0; p < (polyline.size() - 1); ++p)
			//		result[p] = AxW::Geometry::Vector::Unitise(Vector::Perpindicular(polyline[p + 1] - polyline[p], ccw));

			//	if (isLoop)
			//		result.back() = AxW::Geometry::Vector::Unitise(Vector::Perpindicular(polyline.front() - polyline.back(), ccw));

			//	return result;
			//}

			template <class T>
			static std::vector<T> MidPoints(const std::vector<T>& polyline)
			{
				std::vector<T> result(polyline.size() - 1);
				for (unsigned int s = 0; s < polyline.size() - 1; ++s)
					result[s] = Vector::Mid<T>(polyline[s], polyline[s + 1]);
				return result;
			}

			// Sub-loop
			template <class T>
			std::vector<T> SubLoop(const std::vector<T>& loop, unsigned int startIndex, unsigned int endIndex)
			{
				// Loop around...
				if (startIndex < endIndex)
					return std::vector<T>(loop.begin() + startIndex, loop.begin() + endIndex);

				std::vector<T> result(loop.begin() + startIndex, loop.end());
				result.insert(result.end(), loop.begin(), loop.begin() + endIndex);
				return result;
			}

			// Share Vertices...
			std::vector<Vector2> ShareVertices(std::vector<Segment*>& segments, const std::vector<Vector2>& vertices, float tolerance);

			// Loop

			// Winding
			bool LoopIsCCW(const std::vector<Vector2>& polyline);
			bool LoopIsCCW(const std::vector<Vector3>& polyline, const Vector3& origin, const Vector3& normal);

			template <class T>
			static bool IsClosed(const std::vector<T>& polyline, float tolerance)
			{
				return Vector::Equals(polyline.front(), polyline.back(), tolerance);
			}

			// Loop Encloses
			enum PointInLoopResult
			{
				NotInLoop = 0,
				EqualVertex,
				InLoop
			};

			PointInLoopResult LoopEnclosed(const std::vector<Vector2>& loop, const Vector2& point);
			bool LoopEnclosed(const std::vector<Vector2>& loop, const std::vector<Vector2>& points);

			// Lengths
			template <class T>
			float Length(const std::vector<T>& polyline, bool close)
			{
				float result = 0.0f;

				if (polyline.size() < 2)
					return result;
				for (unsigned int p = 1; p < polyline.size(); ++p)
					result += Vector::Length(polyline[p] - polyline[p - 1]);
				if (close)
					result += Vector::Length(polyline.back() - polyline.front());
				return result;
			}

			template <class T>
			std::vector<float> Lengths(const std::vector<T>& polyline)
			{
				std::vector<float> result(polyline.size(), 0.0f);
				for (unsigned int p = 1; p < polyline.size(); ++p)
					result[p] = Vector::Length(polyline[p] - polyline[p - 1]);
				return result;
			}

			template <class T>
			std::vector<float> LengthsAbsolute(const std::vector<T>& polyline)
			{
				std::vector<float> result(polyline.size(), 0.0f);
				for (unsigned int p = 1; p < polyline.size(); ++p)
					result[p] = result[p - 1] + Vector::Length(polyline[p] - polyline[p - 1]);
				return result;
			}

			template <class T>
			float Length(const std::vector<T>& points, const std::list<Polyline::Segment*>& segments)
			{
				float result = 0.0f;
				for (auto segment = segments.begin(); segment != segments.end(); ++segment)
					result += Vector::Length(points[(*segment)->i_] - points[(*segment)->j_]);
				return result;
			}


			// SplitLoop
			std::list<VertexSet2> SplitLoop(const VertexSet2& points, const Vector2& split1, const Vector2& split2);
			std::list<VertexSet3> SplitLoop(const VertexSet3& points, const Vector3& split1, const Vector3& split2);

			// Simplify
			std::vector<unsigned int> RamerDouglasPeucker(const std::vector<Vector2>& points, float distanceTolerance);
			std::vector<unsigned int> RamerDouglasPeucker(const std::vector<Vector3>& points, float distanceTolerance);

			// Spline...
			enum CurveType
			{
				CurveNone = 0,
				CurveCatmullRom,
				CurveBezier,
				CurveBSpline
			};

			std::vector<Vector3> Spline(const std::vector<Vector3>& knots, const CurveType& curveType, int steps);


			//// Average skeleton...
			//struct SkeletonResult
			//{
			//	SkeletonResult(unsigned int size)
			//		: u_(size), v_(size)
			//	{
			//	}

			//	std::vector<AxW::Geometry::Vector3> u_;
			//	std::vector<AxW::Geometry::Vector3> v_;

			//	std::vector<AxW::Geometry::Vector3> Mean()
			//	{
			//		std::vector<AxW::Geometry::Vector3> result(u_.size());
			//		for (unsigned int p = 0; p < u_.size(); ++p)
			//			result[p] = (u_[p] + v_[p]) * 0.5f;
			//		return result;
			//	}

			//	unsigned int size() const { return static_cast<unsigned int>(u_.size()); }
			//};

			//SkeletonResult Skeleton(const std::vector<AxW::Geometry::Vector3>& loop, const AxW::Geometry::Vector3& origin, const Geometry::OrthogonalAxis& axis, int sampleCount);

			// Resampling...
			//std::vector<AxW::Geometry::Vector3> Resample(const std::vector<AxW::Geometry::Vector3>& points, float distance);

			// Preserved resampling... Reduce colinear segments and resample at required distance

			template <class T>
			struct Parametric
			{
			public:
				Parametric(const std::vector<T>& points)
					: points_(points), lengths_(LengthsAbsolute<T>(points_)), parametric_(points.size(), 0.0f)
				{
					for (unsigned int p = 0; p < points.size(); ++p)
						parametric_[p] = lengths_[p] / lengths_.back();
				}

				T Point(float parametricValue) const
				{
					if (!parametricValue)
						return points_.front();
					else if (parametricValue >= 1.0f)
						return points_.back();

					for (unsigned int p = 1; p < points_.size(); ++p)
					{
						if (parametricValue < parametric_[p] && parametricValue >= parametric_[p - 1])
							return points_[p - 1] + ((points_[p] - points_[p - 1]) * ((parametricValue - parametric_[p - 1]) / (parametric_[p] - parametric_[p - 1])));
					}
					return points_.back();
				}

				std::pair<int, int> Indexes(float parametricValue)
				{
					if (parametricValue < parametric_[1])
						return std::make_pair(0, 1);

					for (unsigned int p = 1; p < points_.size(); ++p)
					{
						if (parametricValue < parametric_[p] && parametricValue > parametric_[p - 1])
							return std::make_pair(p - 1, p);
					}
					return std::make_pair(points_.size() - 2, points_.size() - 1);
				}

				std::vector<T> points_;
				std::vector<float> lengths_;
				std::vector<float> parametric_;
			};

			template <class T>
			T PolylinePoint(const std::vector<T>& points, const std::vector<float>& lengths, float length)
			{
				for (unsigned int p = 1; p < points.size(); ++p)
				{
					if (length <= lengths[p] && length >= lengths[p - 1])
					{
						// Interpolate...
						float u = (length - lengths[p - 1]) / (lengths[p] - lengths[p - 1]);
						return points[p - 1] + ((points[p] - points[p - 1]) * u);
					}
				}
				return points.back();
			}

			template <class T>
			T ParametricPoint(const std::vector<T>& points, const std::vector<float>& lengths, float value)
			{
				if (!value)
					return points.front();
				else if (value >= 1.0f)
					return points.back();

				float length = lengths.back() * value;

				for (unsigned int p = 1; p < points.size(); ++p)
				{
					if (length <= lengths[p] && length >= lengths[p - 1])
					{
						// Interpolate...
						float u = (length - lengths[p - 1]) / (lengths[p] - lengths[p - 1]);
						return points[p - 1] + ((points[p] - points[p - 1]) * u);
					}
				}
				return points.back();
			}

			// N.B Last segment might not be spaced like the others...
			template <class T>
			static std::vector<T> Resample(const std::vector<T>& points, float spacing)
			{
				std::list<T> result;
				for (unsigned int p = 1; p < points.size(); ++p)
				{
					T segment = points[p] - points[p - 1];
					float segmentLength = Vector::Length(segment);
					float currentLength = 0;
					segment = Vector::Unitise(segment);

					while (currentLength < (segmentLength - 0.01f))
					{
						result.emplace_back(points[p - 1] + (segment * currentLength));
						currentLength += spacing;
					}
				}
				result.emplace_back(points.back());
				return { result.begin(), result.end() };
			}

			// Make all points equidistance (all segments lengths the same), will be close as possible to 'spacing' but only if legnth/spacing = integer.
			// Any remainder will denote "segment spacing < spacing".
			template <class T>
			std::vector<T> Equidistant(const std::vector<T>& polyline, float spacing)
			{
				if (spacing == 0.0f)
					return polyline;

				auto parametric = Parametric<T>(polyline);

				std::list<T> result;
				float step = abs(spacing) / parametric.lengths_.back();
				int resultCount = static_cast<int>(parametric.lengths_.back() / abs(spacing)) + 1;

				for (unsigned int r = 0; r < resultCount; ++r)
					result.emplace_back(parametric.Point(static_cast<float>(r) * step));

				return { result.begin(), result.end() };
			}

			template <class T>
			std::vector<T> Space(const std::vector<T>& polyline, float spacing)
			{
				if (spacing == 0.0f)
					return polyline;

				// if spacing value > 0, at most spacing distance will be between points...
				// if spacing value < 0, forced to spacing distance between points...
				return spacing > 0 ? Resample<T>(polyline, spacing) : Equidistant<T>(polyline, abs(spacing));
			}

			template <class T>
			bool Collinear(const T& i, const T& j, const T& k, float equalTolerance)
			{
				if (Vector::Equals(i, j, equalTolerance) || Vector::Equals(j, k, equalTolerance) || Vector::Equals(i, k, equalTolerance))
					return true;

				return !Triangle::Area<T>(i, j, k);
			}

		}
	}
}


#endif // PX_GEOMETRY_POLYLINE_HPP