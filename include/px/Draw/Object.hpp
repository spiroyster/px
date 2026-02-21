//#ifndef PX_DRAW_OBJECT_HPP
//#define PX_DRAW_OBJECT_HPP
//
//#include "../Type.hpp"
//
//#include <vector>
//
//namespace px
//{
//	class DrawInterface
//	{
//	public:
//		virtual void Indices(std::vector<unsigned int>& indices) = 0;
//		virtual void Points(std::vector<float>& points, int dimension = 2) = 0;
//		virtual void Normals(std::vector<float>& normals, int dimension = 2) = 0;
//
//		// Generic buffer objects...passed to both vertex and fragment shader
//		virtual void Buffer(const String& name, std::vector<float>& buffer, int dimension = 2) = 0;
//		virtual void Buffer(const String& name, std::vector<int>& buffer, int dimension = 2) = 0;
//	};
//
//
//	class DrawObject
//	{
//	public:
//		DrawObject(const String& name);
//		virtual ~DrawObject();
//
//		virtual void Draw(DrawInterface& drawInterface)
//		{
//
//		}
//
//		virtual Matrix4 Transform() const { return Matrix4(); }
//
//		const String& DrawObjectName() const { return name_; }
//
//		//virtual const Material& 
//
//
//
//		// Points, Indices and normals
//
//		// Additional buffers (normals, colours etc)... 
//		// passed to vertex shader (transformed)
//		// pass to pixel shader
//
//		// Transform (per buffer)
//
//		// Material
//
//	private:
//		String name_;
//	};
//}
//
//#endif // PX_DRAW_OBJECT_HPP 