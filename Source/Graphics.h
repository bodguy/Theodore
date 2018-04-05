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

	private:
		Graphics();
		~Graphics();
    };
}

#endif /* Graphics_h */
