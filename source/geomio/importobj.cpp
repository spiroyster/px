#include <include/px.hpp>

#include "objio.hpp"

PX_COMMAND(importobj)
{
	auto filename = args.back();

	px::Message(px::Str("Loading ") + filename);

	auto obj = objio::readFile(px::ToStr(filename));

	px::Message(px::Str("Points: " + std::to_string(obj->p_.size())));
	px::Message(px::Str("Normals: " + std::to_string(obj->n_.size())));
	px::Message(px::Str("Faces: " + std::to_string(obj->faces_.size())));
	px::Message(px::Str("UVs: " + std::to_string(obj->uv_.size())));
	px::Message(px::Str("Colours: " + std::to_string(obj->c_.size())));



	//else
	//{
	//	/*auto& attrib = objReader.GetAttrib();
	//	auto& shapes = objReader.GetShapes();
	//	for (unsigned int i = 0; i < (attrib.vertices.size() / 3); i++)
	//	{
	//		meshPoints.push_back(AxW::Geometry::Vector3D(attrib.vertices[3 * i], attrib.vertices[(3 * i) + 1], attrib.vertices[(3 * i) + 2]));
	//	}
	//	for (unsigned int i = 0; i < (attrib.normals.size() / 3); i++)
	//	{
	//		meshNormals.push_back(AxW::Geometry::Vector3D(attrib.normals[3 * i], attrib.normals[(3 * i) + 1], attrib.normals[(3 * i) + 2]));
	//	}

	//	for (auto s = shapes.begin(); s != shapes.end(); s++)
	//		for (unsigned int i = 0; i < (s->mesh.indices.size() / 3); i++)
	//		{
	//			meshTriangles.push_back(AxW::Mesh::TriangleMesh::Triangle(s->mesh.indices[3 * i].vertex_index, s->mesh.indices[(3 * i) + 1].vertex_index, s->mesh.indices[(3 * i) + 2].vertex_index));
	//		}*/
	//}

	/*std::vector<AxW::Geometry::Vector3D> meshPoints, meshNormals;
	std::vector<AxW::Mesh::TriangleMesh::Triangle> meshTriangles;

	tinyobj::ObjReader objReader;
	objReader.ParseFromFile(filename, {});
	if (!objReader.Error().empty())
		AxW::Message(objReader.Error());
	else
	{
		auto& attrib = objReader.GetAttrib();
		auto& shapes = objReader.GetShapes();
		for (unsigned int i = 0; i < (attrib.vertices.size() / 3); i++)
		{
			meshPoints.push_back(AxW::Geometry::Vector3D(attrib.vertices[3 * i], attrib.vertices[(3 * i) + 1], attrib.vertices[(3 * i) + 2]));
		}
		for (unsigned int i = 0; i < (attrib.normals.size() / 3); i++)
		{
			meshNormals.push_back(AxW::Geometry::Vector3D(attrib.normals[3 * i], attrib.normals[(3 * i) + 1], attrib.normals[(3 * i) + 2]));
		}

		for (auto s = shapes.begin(); s != shapes.end(); s++)
			for (unsigned int i = 0; i < (s->mesh.indices.size() / 3); i++)
			{
				meshTriangles.push_back(AxW::Mesh::TriangleMesh::Triangle(s->mesh.indices[3 * i].vertex_index, s->mesh.indices[(3 * i) + 1].vertex_index, s->mesh.indices[(3 * i) + 2].vertex_index));
			}
	}

	return AxW::Mesh::Mesh3D({ meshPoints.begin(), meshPoints.end() }, { meshNormals.begin(), meshNormals.end() }, {}, { meshTriangles.begin(), meshTriangles.end() });*/
	

	return true;
}