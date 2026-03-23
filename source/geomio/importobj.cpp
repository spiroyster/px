#include <include/px.hpp>
#include <include/px/Util.hpp>
#include <filesystem>

#include "geomio.hpp"
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

	// Create our draw object(s)
	px::String projectName = px::Str(std::filesystem::path(filename).filename().c_str());

	auto entity = std::make_shared<geomio>(projectName, 
		px::Geometry::TriangleMesh3(
			{
				px::Fill<px::Geometry::Vector3, objio::vector3>(obj->p_, [](const objio::vector3& v) { return px::Geometry::Vector3{ static_cast<float>(v.x_), static_cast<float>(v.y_), static_cast<float>(v.z_) }; }),
				px::Fill<px::Geometry::Vector3, objio::vector3>(obj->n_, [](const objio::vector3& v) { return px::Geometry::Vector3{ static_cast<float>(v.x_), static_cast<float>(v.y_), static_cast<float>(v.z_) }; }),
				px::Fill<px::Geometry::Vector2, objio::vector2>(obj->uv_,[](const objio::vector2& v) { return px::Geometry::Vector2{ static_cast<float>(v.x_), static_cast<float>(v.y_) }; })
			},
			px::Fill<px::Geometry::Mesh::Triangle, objio::face>(obj->faces_, [](const objio::face& f) {
				return
					f.vertices_.size() >= 3 ?
					px::Geometry::Mesh::Triangle{ static_cast<unsigned int>(f.vertices_[0].p_), static_cast<unsigned int>(f.vertices_[0].p_), static_cast<unsigned int>(f.vertices_[0].p_) } :
					px::Geometry::Mesh::Triangle();
				}
			)
		)
	);

	px::AddDataModel(std::make_shared<px::DataModel>(projectName));
	auto dataModel = px::DataModelHandle(projectName);
	dataModel.Mutable().AddEntity(std::static_pointer_cast<px::Entity>(entity));

	return true;
	
	/*tinyobj::ObjReader objReader;
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