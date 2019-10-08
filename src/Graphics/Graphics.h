// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Graphics_h
#define Graphics_h

#include "Asset/Texture.h"
#include "Enumeration.h"
#include "Math/Matrix4x4.h"

namespace Theodore {
  class Vector3d;
  class Color;
  class VertexArray;
  class FrameBuffer;
  class Pipeline;
  class Buffer;
  class Camera;
  class Graphics {
  public:
    static void SetGraphicsSettings();
    static void Dispose();
    static void ClearColor(const Color& color, BufferBits bits);
    static void Clear(BufferBits bits);
    static void BindTexture(unsigned int unit, Texture* texture);
    static void BindFrameBuffer(FrameBuffer* buffer);
    static void DrawArrays(const VertexArray& vao, Primitive mode, unsigned int offset, unsigned int vertices);
    static void DrawElements(const VertexArray& vao, Primitive mode, unsigned int offset, unsigned int count, IndexFormat format);
    static void DrawArraysInstanced(const VertexArray& vao, Primitive mode, unsigned int offset, unsigned int vertices, unsigned int count);
    static void DrawElementsInstanced(const VertexArray& vao, Primitive mode, unsigned int offset, unsigned int count, unsigned int type, unsigned int primcount);
    static void GetLastError();
    static void SetPolygonMode(FillMode value);
    static void Enable(Capabilities value);
    static void Disable(Capabilities value);
    static void PatchParameter(PatchParameters param, int value);
    static void PatchParameter(PatchParameters param, float* values);
    static void ScreenToImage(const std::string& filename, ImageType type);
    static void GetViewport(int* viewport);
    static int GetMaxColorBuffer();
    static int GetMaxDrawBuffer();
    static int GetMaxFrameBufferWidth();
    static int GetMaxFrameBufferHeight();
    static int GetMaxFrameBufferSamples();
    static int GetMaxFrameBufferLayers();
    static void SetFaceCulling(CullFace whichFace, CullMode front);
    static void DrawLine(const Vector3d& start, const Vector3d& end, const Color color);
    static void DrawCube(const Vector3d& center, const Vector3d& size, const Color color, const Matrix4x4 model = Matrix4x4::Identity());
    static void DrawSphere(const Vector3d& center, float radius, const Color color);
    static void DrawFrustum(const Vector3d& center, float fov, float maxRange, float minRange, float aspect, const Color color);
    static void DrawFrustum(const Camera* camera, const Color color);

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
    static void SetMatrixMode(MatrixMode mode);
    static void PopMatrix();
    static void PushMatirx();
    static void Translate(const Vector3d& pos);
    static void Scale(const Vector3d& scale);
    static void Rotate(float r);
    static void SetColor(const Color& color);
    static void Flush();
    static void Set2DGraphics(int w, int h);
    static void SetBlendMode(BlendMode type);

  private:
    Graphics();
    ~Graphics();

    static Pipeline* gizmoProgram;
    static Pipeline* sphereProgram;
    static Buffer* gizmoBuffer;
    static VertexArray* gizmoVao;
  };
}  // namespace Theodore

#endif /* Graphics_h */
