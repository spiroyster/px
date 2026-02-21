#ifndef PX_GEOMETRY_MESH_HPP
#define PX_GEOMETRY_MESH_HPP

#include "Polyline.hpp"

namespace px
{
	namespace Geometry
	{
		namespace Mesh
		{
			struct Edge : Polyline::Segment
			{
				Edge(unsigned int i, unsigned int j) : Polyline::Segment(i, j) {}
				Edge(unsigned int i, unsigned int j, const std::list<unsigned int>& connected) : Polyline::Segment(i, j), connected_(connected) {}
	
				std::list<unsigned int> connected_;		// list of connected meshed objects to this edge (i.e triangles or tetrahedrons)
			};
	
			struct Triangle
			{
				Triangle() : a_(0), b_(0), c_(0) {}
				Triangle(unsigned int a, unsigned int b, unsigned int c) : a_(a), b_(b), c_(c) {}
	
				static Triangle Flip(const Triangle& t) { return Triangle(t.A(), t.C(), t.B()); }
	
				unsigned int A() const { return a_; }
				unsigned int B() const { return b_; }
				unsigned int C() const { return c_; }
			private:
				unsigned int a_, b_, c_;
			};
	
			struct Tetrahedron
			{
				Tetrahedron() : a_(0), b_(0), c_(0), d_(0) {}
				Tetrahedron(unsigned int a, unsigned int b, unsigned int c, unsigned int d) : a_(a), b_(b), c_(c), d_(d) {}
	
				unsigned int A() const { return a_; }
				unsigned int B() const { return b_; }
				unsigned int C() const { return c_; }
				unsigned int D() const { return d_; }
	
			private:
				unsigned int a_, b_, c_, d_;
			};
	
		}
	
		class TriangleMesh
		{
		public:
			TriangleMesh(const std::vector<Mesh::Triangle>& triangles) : triangles_(triangles) {}
	
			unsigned int TriangleCount() const { return static_cast<unsigned int>(triangles_.size()); }
			const std::vector<Mesh::Triangle>& TrianglesImmutable() const { return triangles_; }
			std::vector<Mesh::Triangle>& TrianglesMutable() { return triangles_; }
	
		protected:
			std::vector<Mesh::Triangle> triangles_;
		};
	
	
		class TriangleMesh2 : public TriangleMesh, public VertexSet<Vector2>
		{
		public:
			TriangleMesh2()
				: VertexSet<Vector2>({}), TriangleMesh({})
			{
			}
	
			TriangleMesh2(const VertexSet<Vector2>& vertices, const std::vector<Mesh::Triangle>& triangles)
				: VertexSet<Vector2>(vertices), TriangleMesh(triangles)
			{
			}
	
			void Append(const TriangleMesh2& mesh)
			{
				unsigned int pointCount = PointCount();
				unsigned int triangleCount = TriangleCount();
	
				points_.insert(points_.end(), mesh.points_.begin(), mesh.points_.end());
				normals_.insert(normals_.end(), mesh.normals_.begin(), mesh.normals_.end());
				triangles_.insert(triangles_.end(), mesh.triangles_.begin(), mesh.triangles_.end());
				uvs_.insert(uvs_.end(), mesh.uvs_.begin(), mesh.uvs_.end());
	
				for (unsigned int t = triangleCount; t < triangles_.size(); ++t)
					triangles_[t] = Mesh::Triangle(triangles_[t].A() + pointCount, triangles_[t].B() + pointCount, triangles_[t].C() + pointCount);
	
				CalculateAABB();
			}
	
			std::shared_ptr<Geometric> DeepCopy() const { return std::make_shared<TriangleMesh2>(*this, triangles_); }
	
		};
	
		class TriangleMesh3 : public TriangleMesh, public VertexSet3
		{
		public:
			TriangleMesh3()
				: VertexSet<Vector3>({}), TriangleMesh({})
			{
			}
	
			TriangleMesh3(const VertexSet3& vertices, const std::vector<Mesh::Triangle>& triangles)
				: VertexSet3(vertices), TriangleMesh(triangles)
			{
			}
	
			void Append(const TriangleMesh3& mesh)
			{
				unsigned int pointCount = PointCount();
				unsigned int triangleCount = TriangleCount();
	
				points_.insert(points_.end(), mesh.points_.begin(), mesh.points_.end());
				normals_.insert(normals_.end(), mesh.normals_.begin(), mesh.normals_.end());
				triangles_.insert(triangles_.end(), mesh.triangles_.begin(), mesh.triangles_.end());
				uvs_.insert(uvs_.end(), mesh.uvs_.begin(), mesh.uvs_.end());
	
				for (unsigned int t = triangleCount; t < triangles_.size(); ++t)
					triangles_[t] = Mesh::Triangle(triangles_[t].A() + pointCount, triangles_[t].B() + pointCount, triangles_[t].C() + pointCount);
	
				CalculateAABB();
			}
	
			std::shared_ptr<Geometric> DeepCopy() const { return std::make_shared<TriangleMesh3>(*this, triangles_); }
		};
	
		class TetrahedronMesh : public VertexSet3
		{
		public:
	
			TetrahedronMesh() {}
	
			TetrahedronMesh(const VertexSet3& vertices, const std::vector<Mesh::Tetrahedron>& tetrahedrons) : VertexSet3(vertices), tetrahedrons_(tetrahedrons) {}
	
			unsigned int TetrahedronCount() const { return static_cast<unsigned int>(tetrahedrons_.size()); }
			const std::vector<Mesh::Tetrahedron>& TetrahedronsImmutable() const { return tetrahedrons_; }
			std::vector<Mesh::Tetrahedron>& TetrahedronsMutable() { return tetrahedrons_; }
	
			void Append(const TetrahedronMesh& mesh)
			{
				unsigned int pointCount = PointCount();
				unsigned int tetrahedronCount = TetrahedronCount();
	
				points_.insert(points_.end(), mesh.points_.begin(), mesh.points_.end());
				normals_.insert(normals_.end(), mesh.normals_.begin(), mesh.normals_.end());
				tetrahedrons_.insert(tetrahedrons_.end(), mesh.tetrahedrons_.begin(), mesh.tetrahedrons_.end());
				uvs_.insert(uvs_.end(), mesh.uvs_.begin(), mesh.uvs_.end());
	
				for (unsigned int t = tetrahedronCount; t < tetrahedrons_.size(); ++t)
					tetrahedrons_[t] = Mesh::Tetrahedron(tetrahedrons_[t].A() + pointCount, tetrahedrons_[t].B() + pointCount, tetrahedrons_[t].C() + pointCount, tetrahedrons_[t].D() + pointCount);
	
				CalculateAABB();
			}
	
			std::shared_ptr<Geometric> DeepCopy() const { return std::make_shared<TetrahedronMesh>(*this, tetrahedrons_); }
	
		protected:
			std::vector<Mesh::Tetrahedron> tetrahedrons_;
		};
	}
}

#endif // PX_GEOMETRY_MESH_HPP