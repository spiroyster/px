#ifndef PX_GEOMETRY_VERTEXSET_HPP
#define PX_GEOMETRY_VERTEXSET_HPP

#include "AABB.hpp"

namespace px
{
	namespace Geometry
	{
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

		template <class T>
		class VertexSet : public Geometric
		{
		public:
			VertexSet() {}
			VertexSet(const std::vector<T>& points) : aabb_(points), points_(points) {}
			VertexSet(const std::vector<T>& points, const std::vector<T>& normals) : aabb_(points), points_(points), normals_(normals) {}
			VertexSet(const std::vector<T>& points, const std::vector<T>& normals, const std::vector<Vector2>& uvs) : aabb_(points), points_(points), normals_(normals), uvs_(uvs) {}

			unsigned int PointCount() const { return static_cast<unsigned int>(points_.size()); }
			unsigned int NormalCount() const { return static_cast<unsigned int>(normals_.size()); }
			unsigned int UVCount() const { return static_cast<unsigned int>(uvs_.size()); }

			const std::vector<T>& PointsImmutable() const { return points_; }
			const std::vector<T>& NormalsImmutable() const { return normals_; }
			const std::vector<Vector2>& UVsImmutable() const { return uvs_; }

			std::vector<T>& PointsMutable() { return points_; }
			std::vector<T>& NormalsMutable() { return normals_; }
			std::vector<Vector2>& UVsMutable() { return uvs_; }

			const AABB& AABBImmutable() const { return aabb_; }
			void CalculateAABB() { aabb_ = AABB(points_); }

			std::shared_ptr<Geometric> DeepCopy() const { return std::make_shared<VertexSet<T>>(points_, normals_, uvs_); }

		protected:
			AABB aabb_;

			std::vector<T> points_;
			std::vector<T> normals_;
			std::vector<Vector2> uvs_;
		};

		typedef VertexSet<Vector2> VertexSet2;
		typedef VertexSet<Vector3> VertexSet3;
	}
}

#endif // PX_GEOMETRY_VERTEXSET_HPP