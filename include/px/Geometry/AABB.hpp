#ifndef PX_GEOMETRY_AABB_HPP
#define PX_GEOMETRY_AABB_HPP

#include "Vector.hpp"

#include <vector>
#include <list>
#include <array>
#include <memory>

namespace px
{
	namespace Geometry
	{
		class AABB
		{
			Vector3 min_, max_;
		public:
	
			AABB()
				:min_(Vector3{ 0.0f, 0.0f, 0.0f }), max_(Vector3{ -1.0f, -1.0f, -1.0f })
			{
			}
	
			AABB(const Vector3& min, const Vector3& max)
				: min_(min), max_(max)
			{
			}
	
			AABB(const std::vector<Vector2>& points)
				:min_(Vector3{ 0.0f, 0.0f, 0.0f }), max_(Vector3{ -1.0f, -1.0f, -1.0f })
			{
				if (points.size())
				{
					min_ = max_ = Vector3{points.front().x_, points.front().y_, 0.0f};
					for (auto pt = points.begin(); pt != points.end(); ++pt)
					{
						AddPoint(*pt);
					}
				}
			}
	
			AABB(const std::vector<Vector3>& points)
				:min_(Vector3{ 0.0f, 0.0f, 0.0f }), max_(Vector3{ -1.0f, -1.0f, -1.0f })
			{
				if (points.size())
				{
					min_ = max_ = points.front();
					for (auto pt = points.begin(); pt != points.end(); ++pt)
					{
						AddPoint(*pt);
					}
				}
			}
	
			/// <summary>
			/// The min point of the bounding box. This could be considered as, but strictly enforced as, origin.
			/// </summary>
			/// <returns></returns>
			const Vector3& Min() const { return min_; }
	
			/// <summary>
			/// The max point of the bounding box. For this AABB to be valid, each component of max should be >= to is equivalent component in min.
			/// </summary>
			/// <returns></returns>
			const Vector3& Max() const { return max_; }
	
			Vector3 Centre() const
			{
				return Vector3
				{ 
					(max_.x_ + min_.x_) * 0.5f,
					(max_.y_ + min_.y_) * 0.5f, 
					(max_.z_ + min_.z_) * 0.5f, 
				};
			}
	
			Vector3 Ranges() const
			{
				return Vector3
				{ 
					max_.x_ - min_.x_, 
					max_.y_ - min_.y_, 
					max_.z_ - min_.z_ 
				};
			}
	
			void AddPoint(const Vector2& point)
			{
				if (IsValid())
				{
					min_.x_ = std::min(point.x_, min_.x_);
					min_.y_ = std::min(point.y_, min_.y_);
					max_.x_ = std::max(point.x_, max_.x_);
					max_.y_ = std::max(point.y_, max_.y_);
				}
				else
				{
					max_ = min_ = Vector3{point.x_, point.y_, 0.0f};
				}
			}
	
			void AddPoint(const Vector3& point)
			{
				if (IsValid())
				{
					min_.x_ = std::min(point.x_, min_.x_);
					min_.y_ = std::min(point.y_, min_.y_);
					min_.z_ = std::min(point.z_, min_.z_);
					max_.x_ = std::max(point.x_, max_.x_);
					max_.y_ = std::max(point.y_, max_.y_);
					max_.z_ = std::max(point.z_, max_.z_);
				}
				else
				{
					max_ = min_ = point;
				}
			}
	
			bool IsValid() const
			{
				return max_.x_ >= min_.x_ && max_.y_ >= min_.y_ && max_.z_ >= min_.z_;
			}
	
			std::array<Vector3, 8> GetPoints() const
			{
				return {
					min_, Vector3{ min_.x_, max_.y_, min_.z_ }, Vector3{ max_.x_, max_.y_, min_.z_ }, Vector3{ max_.x_, min_.y_, min_.z_},
					Vector3{ min_.x_, min_.y_, max_.z_ }, Vector3{ min_.x_, max_.y_, max_.z_ }, max_, Vector3{ max_.x_, min_.y_, max_.z_}
				};
			}
	
			float Radius() const
			{
				return Vector::Length(Vector::Subtract(max_, min_)) * 0.5f;
			}
	
			bool EnclosesPoint(const Vector3& p) const
			{
				return (p.x_ >= min_.x_) && (p.y_ >= min_.y_) && (p.z_ >= min_.z_) && (p.x_ <= max_.x_) && (p.y_ <= max_.y_) && (p.z_ <= max_.z_);
			}
	
			void Union(const AABB& aabb)
			{
				if (!IsValid())
					*this = aabb;
				else if (aabb.IsValid())
				{
					AddPoint(aabb.Max());
					AddPoint(aabb.Min());
				}
			}
	
			float Volume() const
			{
				auto ranges = Ranges();
				return ranges.x_ * ranges.y_ * ranges.z_;
			}
	
			void Inflate(const Vector3& inflation)
			{
				max_ += (inflation * 0.5f);
				min_ -= (inflation * 0.5f);
			}
	
			bool Intersect(const AABB& aabb) const
			{
				if (max_.x_ < aabb.Min().x_)
					return false;
				if (max_.y_ < aabb.Min().y_)
					return false;
				if (max_.z_ < aabb.Min().z_)
					return false;
	
				if (min_.x_ > aabb.Max().x_)
					return false;
				if (min_.y_ > aabb.Max().y_)
					return false;
				if (min_.z_ > aabb.Max().z_)
					return false;
	
				return true;
			}
	
			bool Intersect(const Vector3& a, const Vector3& b, const Vector3& c) const
			{
				// check if the triangle intersects this AABB...
				AABB triangleAABB(std::vector<Vector3>({ a, b, c }));
	
				if (!Intersect(triangleAABB))
					return false;
	
				// Otherwise we need to check if the triangle intersects
				return true;
			}
	
			std::list<Vector3> Intersect(const Vector3& position, const Vector3& direction) const
			{
				// If the position is already in the AABB, this is the result...
				if (EnclosesPoint(position))
					return { position };

				Vector3 AxisX = Vector3{ 1.0f, 0.0f, 0.0f };
				Vector3 AxisY = Vector3{ 0.0f, 1.0f, 0.0f };
				Vector3 AxisZ = Vector3{ 0.0f, 0.0f, 1.0f };
	
				std::list<Vector3> intersections;
	
				// Check min/max x planes
				std::optional<Vector3> intersectionPoint = Intersect::LinePlane(position, position + direction, min_, AxisX);
				if (intersectionPoint != std::nullopt)
				{
					// x will be valid as it will be on the min x or max x plane of the AABB, so check if y and z are valid...
					if (intersectionPoint->z_ >= min_.z_ && intersectionPoint->z_ <= max_.z_ && intersectionPoint->y_ >= min_.y_ && intersectionPoint->y_ <= max_.y_)
						intersections.emplace_back(*intersectionPoint);
	
					// Check the x max plane...
					intersectionPoint = Intersect::LinePlane(position, position + direction, max_, AxisX);
					if (intersectionPoint != std::nullopt)
					{
						if (intersectionPoint->z_ >= min_.z_ && intersectionPoint->z_ <= max_.z_ && intersectionPoint->y_ >= min_.y_ && intersectionPoint->y_ <= max_.y_)
							intersections.emplace_back(*intersectionPoint);
					}
				}
	
				// Check min/max y planes
				intersectionPoint = Intersect::LinePlane(position, position + direction, min_, AxisY);
				if (intersectionPoint != std::nullopt)
				{
					// y will be valid as it will be on the min y or max y plane of the AABB, so check if x and z are valid...
					if (intersectionPoint->z_ >= min_.z_ && intersectionPoint->z_ <= max_.z_ && intersectionPoint->x_ >= min_.x_ && intersectionPoint->x_ <= max_.x_)
						intersections.emplace_back(*intersectionPoint);
	
					// Check the y max plane...
					intersectionPoint = Intersect::LinePlane(position, position + direction, max_, AxisY);
					if (intersectionPoint != std::nullopt)
					{
						if (intersectionPoint->z_ >= min_.z_ && intersectionPoint->z_ <= max_.z_ && intersectionPoint->x_ >= min_.x_ && intersectionPoint->x_ <= max_.x_)
							intersections.emplace_back(*intersectionPoint);
					}
				}
	
				// Check min/max z planes
				intersectionPoint = Intersect::LinePlane(position, position + direction, min_, AxisZ);
				if (intersectionPoint != std::nullopt)
				{
					// z will be valid as it will be on the min z or max z plane of the AABB, so check if x and y are valid...
					if (intersectionPoint->x_ >= min_.x_ && intersectionPoint->x_ <= max_.x_ && intersectionPoint->y_ >= min_.y_ && intersectionPoint->y_ <= max_.y_)
						intersections.emplace_back(*intersectionPoint);
	
					// Check the z max plane...
					intersectionPoint = Intersect::LinePlane(position, Vector::Add(position, direction), max_, AxisZ);
					if (intersectionPoint != std::nullopt)
					{
						if (intersectionPoint->x_ >= min_.x_ && intersectionPoint->x_ <= max_.x_ && intersectionPoint->y_ >= min_.y_ && intersectionPoint->y_ <= max_.y_)
							intersections.emplace_back(*intersectionPoint);
					}
				}
	
				//// Unique them...
				intersections.sort([](const Vector3& a, const Vector3& b)
					{
						if (a.x_ == b.x_)
						{
							if (a.y_ == b.y_)
								return a.z_ < b.z_;
							return a.y_ < b.y_;
						}
						return a.x_ < b.x_;
					});
				intersections.unique([](const Vector3& a, const Vector3& b)
					{
						return (a.x_ == b.x_ && a.y_ == b.y_ && a.z_ == b.z_);
					});
	
				return intersections;
			}
	
			void Transform(const Matrix4& transform)
			{
				min_ = Matrix::Transform(transform, min_, 1.0f);
				max_ = Matrix::Transform(transform, max_, 1.0f);
			}
	
			AABB Transformed(const Matrix4& transform) const
			{
				return AABB(Matrix::Transform(transform, min_, 1.0f), Matrix::Transform(transform, max_, 1.0f));
			}
	
		};

		class Geometric
		{
		public:
			virtual ~Geometric() {}

			virtual unsigned int PointCount() const = 0;
			virtual unsigned int NormalCount() const = 0;

			virtual const AABB& AABBImmutable() const = 0;
			virtual void CalculateAABB() = 0;

			virtual std::shared_ptr<Geometric> DeepCopy() const = 0;
		};

	}	// namespace Geometry
}		// namespace px

#endif // PX_GEOMETRY_AABB_HPP