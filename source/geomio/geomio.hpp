#ifndef GEOMIO_HPP
#define GEOMIO_HPP

#include <include/px.hpp>

class geomio : 
	public px::DrawObject, 
	public px::Entity
{
public:
	geomio(const px::String& name, const px::Geometry::TriangleMesh3& mesh)
		: px::DrawObject(name), px::Entity(name), mesh_(mesh)
	{
	}

	void Draw(px::DrawInterface& drawInterface)
	{
		//drawInterface.Points(mesh_.PointsImmutable());

	}

private:
	px::Geometry::TriangleMesh3 mesh_;
};

#endif // GEOMIO_HPP