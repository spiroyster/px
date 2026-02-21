#ifndef MODEL_OBJIO_HPP
#define MODEL_OBJIO_HPP

#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>

/// Read and write obj (alias wavefront) file format...
namespace objio
{
	namespace impl
	{
		static std::vector<std::string> split(const std::string& str, const std::string& seperator)
		{
			std::list<std::string> tokens;

			if (str.empty())
				return std::vector<std::string>();

			std::size_t start = 0;
			std::size_t itr = str.find(seperator, start);

			while (itr != std::string::npos)
			{
				std::string token = str.substr(start, itr - start);
				if (!token.empty())
					tokens.push_back(token);
				start = itr + seperator.size();
				itr = str.find(seperator, start);
			}
			tokens.push_back(str.substr(start));

			return std::vector<std::string>(tokens.begin(), tokens.end());
		}

		static std::string rightTrim(const std::string& str, const std::string& chars = "\t\n\v\f\r ")
		{
			std::string newStr = str;
			newStr.erase(newStr.find_last_not_of(chars) + 1);
			return newStr;
		}

		static std::string leftTrim(const std::string& str, const std::string& chars = "\t\n\v\f\r ")
		{
			std::string newStr = str;
			newStr.erase(0, newStr.find_first_not_of(chars));
			return newStr;
		}

		static std::string trim(const std::string& str, const std::string& chars = "\t\n\v\f\r ")
		{
			return leftTrim(rightTrim(str, chars), chars);
		}

		static int correctIndex(int index, int bufferSize)
		{
			if (index < 0)
				return bufferSize + index;
			return index - 1;
		}
	}

	struct vector2
	{
		vector2(double x, double y) : x_(x), y_(y) {}

		double x_, y_;
	};
	struct vector3
	{
		vector3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

		double x_, y_, z_;
	};
	struct vertex
	{
		vertex(int p, int n, int uv, int c) : p_(p), n_(n), uv_(uv), c_(c) {}

		static vertex p(int p) { return vertex(p, -1, -1, -1); }
		static vertex pc(int p, int c) { return vertex(p, 0, 0, c); }
		static vertex pn(int p, int n) { return vertex(p, n, 0, 0); }
		static vertex pt(int p, int t) { return vertex(p, 0, t, 0); }
		static vertex ptc(int p, int t, int c) { return vertex(p, 0, t, c); }
		static vertex pnt(int p, int n, int t) { return vertex(p, n, t, 0); }
		static vertex pnc(int p, int n, int c) { return vertex(p, n, 0, c); }
		static vertex pntc(int p, int n, int t, int c) { return vertex(p, n, t, c); }

		static vertex parse(const std::string& str)
		{
			try
			{
				std::vector<std::string> vertexIndexes = impl::split(str, "/");
				if (vertexIndexes.size() == 1)
					return vertex::p(stoi(vertexIndexes[0]));
				else if (vertexIndexes.size() == 2)
					return str.find("//") != std::string::npos ? vertex::pn(stoi(vertexIndexes[0]), stoi(vertexIndexes[1])) : vertex::pt(stoi(vertexIndexes[0]), stoi(vertexIndexes[1]));
				else if (vertexIndexes.size() == 3)
					return vertex::pnt(stoi(vertexIndexes[0]), stoi(vertexIndexes[2]), stoi(vertexIndexes[1]));
				throw std::runtime_error("Incorrect number of vertex indexes.");
			}
			catch (...)
			{
				int y = 0;
				++y;
				throw std::runtime_error("Uh oh.");
			}
		}

		int p_, n_, uv_, c_;
	};
	struct face
	{
		face(const std::vector<vertex>& vertices)
			: vertices_(vertices)
		{
		}

		static face parse(const std::string& str)
		{
			std::vector<vertex> vertices;
			std::vector<std::string> faceSyntax = impl::split(impl::trim(str), " ");
			std::for_each(faceSyntax.begin(), faceSyntax.end(),
				[&vertices](const std::string& faceStr)
			{
				vertices.push_back(vertex::parse(faceStr));
			});

			return face(vertices);
		}

		const vertex& operator[](unsigned int i) const { return vertices_[i]; }

		std::vector<vertex> vertices_;
	};
	struct mesh
	{
		std::vector<vector3> p_;
		std::vector<vector3> n_;
		std::vector<vector2> uv_;
		std::vector<vector3> c_;
		std::vector<face> faces_;
	};



	template<class T>
	class VertexList
	{
	public:

		void finialise()
		{
			list_.insert(list_.end(), current_.begin(), current_.end());
			current_.clear();
		}

		int correctIndex(int index)
		{
			if (index < 0)
				return static_cast<int>((current_.size() + index) + list_.size());
			return static_cast<int>((list_.size() + index) - 1);
		}

		std::list<T> current_;
		std::list<T> list_;
	};

	namespace CurrentList
	{
		enum CurrentList
		{
			None,
			V,
			VT,
			VN,
			F
		};
	}

	static void checkCurrentVertexList(CurrentList::CurrentList& currentList, const CurrentList::CurrentList& required,
		VertexList<vector3>& points, VertexList<vector3>& normals, VertexList<vector2>& uvs)
	{
		if (currentList != required)
		{
			switch (currentList)
			{
			case CurrentList::VT:
				uvs.finialise();
				break;
				
			case CurrentList::VN:
				normals.finialise();
				break;
				
			case CurrentList::V:
				points.finialise();
				break;
			}
			currentList = required;
		}
	}

	static std::shared_ptr<mesh> read(const std::string& str)
	{
		std::istringstream syntax(str);

		std::string line;

		VertexList<vector3> points, normals;
		VertexList<vector2> uvs;
		std::list<face> faces;

		CurrentList::CurrentList currentList = CurrentList::None;

		while (std::getline(syntax, line))
		{
			line = impl::trim(line);

			// trim comment (if any)...
			std::size_t comment = line.find('#');
			if (comment != std::string::npos)
				line = line.substr(comment);
				
			// Other keywords we are ignoring...
			if (line.find("mtllib") == 0)
				continue;

			if (line.find("usemtl") == 0)
				continue;

			if (line.find("g") == 0)
				continue;

			if (line.find("o") == 0)
				continue;

			if (line.find("s") == 0)
				continue;

			// look for vt (texture coordinates)
			std::size_t vt = line.find("vt");
			if (vt == 0)
			{
				checkCurrentVertexList(currentList, CurrentList::VT, points, normals, uvs);

				std::istringstream iss(line.substr(vt + 2));
				double x = 0, y = 0;
				iss >> std::ws >> x >> std::ws >> y;
				uvs.current_.push_back(vector2(x, y));
				continue;
			}

			// look for vn (normals)
			std::size_t vn = line.find("vn");
			if (vn == 0)
			{
				checkCurrentVertexList(currentList, CurrentList::VN, points, normals, uvs);

				std::istringstream iss(line.substr(vn + 2));
				double x = 0, y = 0, z = 0;
				iss >> std::ws >> x >> std::ws >> y >> std::ws >> z;
				normals.current_.push_back(vector3(x, y, z));
				continue;
			}

			// look for v
			std::size_t v = line.find("v");
			if (v == 0)
			{
				checkCurrentVertexList(currentList, CurrentList::V, points, normals, uvs);

				// tokenize...
				std::vector<std::string> vertex = impl::split(impl::trim(line.substr(v + 1)), " ");
				unsigned int vertexComponentCount = static_cast<unsigned int>(vertex.size());

				// standard vertex x,y,z (w)...
				if (vertexComponentCount == 3)
					points.current_.push_back(vector3(stod(vertex[0]), stod(vertex[1]), stod(vertex[2])));
				else if (vertexComponentCount == 4)
					points.current_.push_back(vector3(stod(vertex[0]), stod(vertex[1]), stod(vertex[2])));
				else if (vertexComponentCount == 6)
				{
					points.current_.push_back(vector3(stod(vertex[0]), stod(vertex[1]), stod(vertex[2])));
					points.current_.push_back(vector3(stod(vertex[0]), stod(vertex[1]), stod(vertex[2])));
				}
				else
					throw std::exception("Number of components in vertex \"v\" not supported.");

				continue;
			}

			// look for f
			std::size_t f = line.find("f");
			if (f == 0)
			{
				faces.push_back(face::parse(impl::trim(line.substr(f + 1))));

				if (faces.size() == 155101)
				{
					int y = 0;
					++y;
				}
				// correct the indexes based on current lists...
				face& fce = faces.back();
				for (unsigned int i = 0; i < fce.vertices_.size(); ++i)
				{
					fce.vertices_[i].p_ = points.correctIndex(fce.vertices_[i].p_);
					fce.vertices_[i].n_ = normals.correctIndex(fce.vertices_[i].n_);
					fce.vertices_[i].uv_ = uvs.correctIndex(fce.vertices_[i].uv_);

					if (fce.vertices_[i].n_ == 131321)
					{
						int y = 0;
						++y;
					}
				}
				continue;
			}
		}

		points.finialise();
		normals.finialise();
		uvs.finialise();

		std::shared_ptr<mesh> result(new mesh);
		result->p_.insert(result->p_.end(), points.list_.begin(), points.list_.end());
		result->n_.insert(result->n_.end(), normals.list_.begin(), normals.list_.end());
		result->uv_.insert(result->uv_.end(), uvs.list_.begin(), uvs.list_.end());
		result->faces_.insert(result->faces_.end(), faces.begin(), faces.end());

		return result;

	}

	static std::shared_ptr<mesh> readFile(const std::string& filename)
	{
		std::ifstream file(filename);
		if (!file)
			throw std::exception("Unable to read file");

		std::string str;

		file.seekg(0, std::ios::end);
		str.reserve(static_cast<size_t>(file.tellg()));
		file.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		return read(str);
	}

	static std::string write(const mesh& mesh)
	{
		std::ostringstream oss;

		oss << "# obj file. 5AxisWorks";

		oss << "\n# points...\n";
		if (mesh.c_.empty())
		{
			std::for_each(mesh.p_.begin(), mesh.p_.end(),
				[&oss](const vector3& v)
			{
				oss << "v " << v.x_ << " " << v.y_ << " " << v.z_ << '\n';
			});
		}
		else if (mesh.p_.size() == mesh.c_.size())
		{
			for (unsigned int i = 0; i < mesh.p_.size(); ++i)
			{
				const vector3& v = mesh.p_[i];
				const vector3& c = mesh.c_[i];
				oss << "v " << v.x_ << " " << v.y_ << " " << v.z_ << " " << c.x_ << " " << c.y_ << " " << c.z_ << '\n';
			}
		}

		if (!mesh.n_.empty())
		{
			oss << "\n# normals...\n";
			std::for_each(mesh.n_.begin(), mesh.n_.end(),
				[&oss](const vector3& v)
			{
				oss << "vn " << v.x_ << " " << v.y_ << " " << v.z_ << '\n';
			});
		}

		if (!mesh.uv_.empty())
		{
			oss << "\n# texture coordinates...\n";
			std::for_each(mesh.uv_.begin(), mesh.uv_.end(),
				[&oss](const vector2& v)
			{
				oss << "vn " << v.x_ << " " << v.y_ << '\n';
			});
		}

		oss << "\n# faces...\n";
		std::for_each(mesh.faces_.begin(), mesh.faces_.end(),
			[&oss](const face& f)
		{
			oss << "f ";
			std::for_each(f.vertices_.begin(), f.vertices_.end(),
				[&oss](const vertex& v)
			{
				oss << v.p_ + 1;
				if (v.n_ != -1 && v.uv_ != -1)
					oss << "/" << v.uv_ + 1 << "/" << v.n_ + 1;
				else if (v.n_ != -1)
					oss << "//" << v.n_ + 1;
				oss << " ";
			});
			oss << '\n';
		});


		return oss.str();
	}

	static void writeFile(const std::string& filename, const mesh& mesh)
	{
		std::ofstream file(filename);
		if (file)
			file << write(mesh);
		else
			throw std::runtime_error("Unable to save obj file.");
	}

}


#endif // MODEL_OBJIO_HPP