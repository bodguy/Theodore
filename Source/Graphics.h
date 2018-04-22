#ifndef Graphics_h
#define Graphics_h

#include "Texture.h"
#include "Enumeration.h"

namespace Quark {
    class Color; class VertexArray; class FrameBuffer;
    class Graphics {
    public:
        static void Clear(const Color& color, Enumeration::BufferBits bits);
        static void BindTexture(unsigned int unit, Texture* texture);
        static void BindFrameBuffer(FrameBuffer* buffer);
        static void DrawArrays(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int vertices);
        static void DrawElements(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int count, unsigned int type);
        static void DrawArraysInstanced(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int vertices, unsigned int count);
        static void DrawElementsInstanced(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int count, unsigned int type, unsigned int primcount);
        static void GetLastError();
        static void SetPolygonMode(Enumeration::FillMode value);
        static void Enable(Enumeration::Capabilities value);
        static void Disable(Enumeration::Capabilities value);
		static void PatchParameter(Enumeration::PatchParameters param, int value);
		static void PatchParameter(Enumeration::PatchParameters param, float* values);
		static void ScreenToImage(const std::string& filename, Enumeration::ImageType type);

#if 0
		// OpenGL 2.1 version rendering functions
		static void Vertex(const Vector3d& vector);
		static void Point(const Vector3d& vector, const Color& color, float thickness = 1.f);
		static void Line(const Vector3d& start, const Vector3d& end, const Color& color, float thickness = 1.f);
		static void Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Color& color, float thickness = 1.f);
		static void SolidTriangle(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Color& color);
		static void Circle(const Vector3d& center, float radius, const Color& color, float thickness = 1.f);
		static void Quad(const Vector3d& center, float w, float h, const Color& color, float thickness = 1.f);
		static void SolidQuad(const Vector3d& center, float w, float h, const Color& color);
		static void TexturedQuad(const Vector3d& center, float w, float h, const Vector3d& offset, const Vector3d& scale, const Color& color);
		static void Polygon(const std::vector<Vector3d>& verts, const Color& color, float thickness = 1.f);

		static void SetDimensions(unsigned int w, unsigned int h);
		static void SetMatrixMode(Enumeration::MatrixMode mode);
		static void PopMatrix();
		static void PushMatirx();
		static void Translate(const Vector3d& pos);
		static void Scale(const Vector3d& scale);
		static void Rotate(float r);
		static void SetColor(const Color& color);
		static void Flush();
		static void Set2DGraphics(int w, int h);
		static void SetBlendMode(Enumeration::BlendMode type);
#endif

	private:
		Graphics();
		~Graphics();
    };
}

#endif /* Graphics_h */
