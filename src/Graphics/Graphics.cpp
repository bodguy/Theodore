#include "Graphics.h"
#include "../Object/Component/Camera.h"
#include "../Math/Color.h"
#include "../Helper/Debug.h"
#include "FrameBuffer.h"
#include "../Math/Math.h"
#include "../Platform/Platform.h"
#include "../Object/SceneManager.h"
#include "../Asset/Shader.h"
#include "../Helper/Utility.h"
#include "VertexBuffer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace Theodore {
  Pipeline* Graphics::gizmoProgram = NULL;
  Pipeline* Graphics::sphereProgram = NULL;
  Buffer* Graphics::gizmoBuffer = NULL;
  VertexArray* Graphics::gizmoVao = NULL;

  Graphics::Graphics() {}

  Graphics::~Graphics() {}

  void Graphics::SetGraphicsSettings() {
    gizmoProgram = Shader::Find("Gizmo");
    sphereProgram = Shader::Find("Sphere");
    gizmoBuffer = new Buffer(BufferType::BufferVertex);
    gizmoBuffer->Data(nullptr, 24 * sizeof(Vector3d), BufferUsage::DynamicDraw);
    gizmoVao = new VertexArray();
    gizmoVao->BindAttribute(gizmoProgram->GetAttribute("position"), *gizmoBuffer, 3,
                            sizeof(Vector3d), 0);
    gizmoVao->BindAttribute(sphereProgram->GetAttribute("position"), *gizmoBuffer, 3,
                            sizeof(Vector3d), 0);
  }

  void Graphics::Dispose() {
    SafeDealloc(gizmoBuffer);
    SafeDealloc(gizmoVao);
    // DO NOT DEINITIALIZE Pipeline objects
  }

  void Graphics::ClearColor(const Color& color, BufferBits bits) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(static_cast<GLbitfield>(bits));
  }

  void Graphics::Clear(BufferBits bits) { glClear(static_cast<GLbitfield>(bits)); }

  void Graphics::BindTexture(unsigned int unit, Texture* texture) {
    glActiveTexture(GL_TEXTURE0 + unit);
    if (texture) {
      glBindTexture(static_cast<GLenum>(texture->GetDimension()), texture->GetTextureID());
    } else {
      glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(NULL));
    }
  }

  void Graphics::BindFrameBuffer(FrameBuffer* buffer) {
    if (buffer) {
      glViewport(0, 0, buffer->GetWidth(), buffer->GetHeight());
      glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetFrameBufferID());
    } else {
      glViewport(0, 0, Platform::GetWidth(), Platform::GetHeight());
      glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    }
  }

  void Graphics::DrawArrays(const VertexArray& vao, Primitive mode, unsigned int offset,
                            unsigned int vertices) {
    glBindVertexArray(vao.GetArrayID());
    glDrawArrays(static_cast<GLenum>(mode), offset, vertices);
    glBindVertexArray(0);
  }

  void Graphics::DrawElements(const VertexArray& vao, Primitive mode, unsigned int offset,
                              unsigned int count, IndexFormat format) {
    glBindVertexArray(vao.GetArrayID());
    glDrawElements(static_cast<GLenum>(mode), count, static_cast<GLenum>(format),
                   reinterpret_cast<const void*>(offset));
    glBindVertexArray(0);
  }

  void Graphics::DrawArraysInstanced(const VertexArray& vao, Primitive mode, unsigned int offset,
                                     unsigned int vertices, unsigned int count) {
    glBindVertexArray(vao.GetArrayID());
    glDrawArraysInstanced(static_cast<GLenum>(mode), offset, vertices, count);
    glBindVertexArray(0);
  }

  void Graphics::DrawElementsInstanced(const VertexArray& vao, Primitive mode, unsigned int offset,
                                       unsigned int count, unsigned int type,
                                       unsigned int primcount) {
    glBindVertexArray(vao.GetArrayID());
    glDrawElementsInstanced(static_cast<GLenum>(mode), count, type,
                            reinterpret_cast<const void*>(offset), primcount);
    glBindVertexArray(0);
  }

  void Graphics::GetLastError() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      switch (err) {
      case GL_INVALID_ENUM: // 0x0500
        Debug::Log("GL_INVALID_ENUM");
        break;
      case GL_INVALID_VALUE: // 0x0501
        Debug::Log("GL_INVALID_VALUE");
        break;
      case GL_INVALID_OPERATION: // 0x0502
        Debug::Log("GL_INVALID_OPERATION");
        break;
      case GL_STACK_OVERFLOW: // 0x0503
        Debug::Log("GL_STACK_OVERFLOW");
        break;
      case GL_STACK_UNDERFLOW: // 0x0504
        Debug::Log("GL_STACK_UNDERFLOW");
        break;
      case GL_OUT_OF_MEMORY: // 0x0505
        Debug::Log("GL_OUT_OF_MEMORY");
        break;
      }
    }
  }

  void Graphics::SetPolygonMode(FillMode value) {
    glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(value));
  }

  void Graphics::Enable(Capabilities value) { glEnable(static_cast<GLenum>(value)); }

  void Graphics::Disable(Capabilities value) { glDisable(static_cast<GLenum>(value)); }

  void Graphics::PatchParameter(PatchParameters param, int value) {
    glPatchParameteri(static_cast<GLenum>(param), value);
  }

  void Graphics::PatchParameter(PatchParameters param, float* values) {
    glPatchParameterfv(static_cast<GLenum>(param), values);
  }

  void Graphics::ScreenToImage(const std::string& filename, ImageType type) {
    int w = Platform::GetWidth();
    int h = Platform::GetHeight();
    char* data = new char[w * h * 3];
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);

    char* tmpline = new char[w * 3];

    // const int linewidth = w * 3;

    // flip the image
    // for (int y = 0; y < (h / 2); y++) {
    //	std::copy(data + y * linewidth, data + y * linewidth + linewidth, tmpline);
    //	std::copy(data + (h - y) * linewidth, data + (h - y) * linewidth + linewidth, data + y *
    // linewidth);
    //	std::copy(tmpline, tmpline + linewidth, data + (h - y) * linewidth);
    //}

    switch (type) {
    case ImageType::IMAGE_PNG:
      stbi_write_png(filename.data(), w, h, 3, data, w * 3);
      break;
    case ImageType::IMAGE_BMP:
      stbi_write_bmp(filename.data(), w, h, 3, data);
      break;
    case ImageType::IMAGE_TGA:
      stbi_write_tga(filename.data(), w, h, 3, data);
      break;
    }

    delete [] tmpline;
    delete [] data;
  }

  void Graphics::GetViewport(int* viewport) { glGetIntegerv(GL_VIEWPORT, viewport); }

  int Graphics::GetMaxColorBuffer() {
    int res = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &res);
    return res;
  }

  int Graphics::GetMaxDrawBuffer() {
    int res = 0;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &res);
    return res;
  }

  int Graphics::GetMaxFrameBufferWidth() {
    int res = 0;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &res);
    return res;
  }

  int Graphics::GetMaxFrameBufferHeight() {
    int res = 0;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &res);
    return res;
  }

  int Graphics::GetMaxFrameBufferSamples() {
    int res = 0;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &res);
    return res;
  }

  int Graphics::GetMaxFrameBufferLayers() {
    int res = 0;
    glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &res);
    return res;
  }

  void Graphics::SetFaceCulling(CullFace whichFace, CullMode front) {
    glCullFace(static_cast<GLenum>(whichFace));
    glFrontFace(static_cast<GLenum>(front));
  }

  void Graphics::DrawLine(const Vector3d& start, const Vector3d& end, const Color color) {
    Vector3d vertices[2] = {start, end};

    gizmoProgram->Use();
    gizmoProgram->SetUniform("model", Matrix4x4::Identity());
    gizmoProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
    gizmoProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    gizmoProgram->SetUniform("color", color);
    gizmoBuffer->SubData(vertices, 0, 2 * sizeof(Vector3d));
    Graphics::DrawArrays(*gizmoVao, Primitive::LineLoop, 0, 2);
    gizmoProgram->UnUse();
  }

  void Graphics::DrawCube(const Vector3d& center, const Vector3d& size, const Color color,
                          const Matrix4x4 model) {
    Vector3d extent = size * 0.5f;
    Vector3d vertices[24] = {
        // top
        center + extent * Vector3d(1.f, 1.f, 1.f), center + extent * Vector3d(-1.f, 1.f, 1.f),
        center + extent * Vector3d(-1.f, 1.f, 1.f), center + extent * Vector3d(-1.f, 1.f, -1.f),
        center + extent * Vector3d(-1.f, 1.f, -1.f), center + extent * Vector3d(1.f, 1.f, -1.f),
        center + extent * Vector3d(1.f, 1.f, -1.f), center + extent * Vector3d(1.f, 1.f, 1.f),

        // vertical
        center + extent * Vector3d(1.f, 1.f, 1.f), center + extent * Vector3d(1.f, -1.f, 1.f),
        center + extent * Vector3d(-1.f, 1.f, 1.f), center + extent * Vector3d(-1.f, -1.f, 1.f),
        center + extent * Vector3d(-1.f, 1.f, -1.f), center + extent * Vector3d(-1.f, -1.f, -1.f),
        center + extent * Vector3d(1.f, 1.f, -1.f), center + extent * Vector3d(1.f, -1.f, -1.f),

        // bottom
        center + extent * Vector3d(-1.f, -1.f, -1.f), center + extent * Vector3d(1.f, -1.f, -1.f),
        center + extent * Vector3d(1.f, -1.f, -1.f), center + extent * Vector3d(1.f, -1.f, 1.f),
        center + extent * Vector3d(1.f, -1.f, 1.f), center + extent * Vector3d(-1.f, -1.f, 1.f),
        center + extent * Vector3d(-1.f, -1.f, 1.f), center + extent * Vector3d(-1.f, -1.f, -1.f)};

    gizmoProgram->Use();
    gizmoProgram->SetUniform("model", model);
    gizmoProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
    gizmoProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    gizmoProgram->SetUniform("color", color);
    gizmoBuffer->SubData(vertices, 0, 24 * sizeof(Vector3d));
    Graphics::DrawArrays(*gizmoVao, Primitive::Lines, 0, 24);
    gizmoProgram->UnUse();
  }

  void Graphics::DrawSphere(const Vector3d& center, float radius, const Color color) {
    sphereProgram->Use();
//    Camera* cam = SceneManager::GetMainCamera();
    sphereProgram->SetUniform("model", Matrix4x4::Identity());
    sphereProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
    sphereProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    sphereProgram->SetUniform("perspective", SceneManager::GetMainCamera()->GetProjectionMatrix());
    sphereProgram->SetUniform("radius", radius);
    sphereProgram->SetUniform("color", color);
    gizmoBuffer->SubData(&center, 0, sizeof(Vector3d));
    Graphics::DrawArrays(*gizmoVao, Primitive::Points, 0, 1);
    sphereProgram->UnUse();
  }

  void Graphics::DrawFrustum(const Vector3d& center, float fov, float maxRange, float minRange,
                             float aspect, const Color color) {
    float tanAngle = Math::Tan(fov / 2);

    float yNear = minRange * tanAngle;
    float xNear = aspect * yNear;
    float yFar = maxRange * tanAngle;
    float xFar = aspect * yFar;

    Vector3d nearLeftTop(-xNear, yNear, -10.0f * minRange);
    Vector3d nearLeftBottom(-xNear, -yNear, -10.0f * minRange);
    Vector3d nearRightBottom(xNear, -yNear, -10.0f * minRange);
    Vector3d nearRightTop(xNear, yNear, -10.0f * minRange);

    Vector3d farLeftTop(-xFar, yFar, -10.0f * maxRange);
    Vector3d farLeftBottom(-xFar, -yFar, -10.0f * maxRange);
    Vector3d farRightBottom(xFar, -yFar, -10.0f * maxRange);
    Vector3d farRightTop(xFar, yFar, -10.0f * maxRange);

    Vector3d vertices[24] = {
        nearLeftTop,     nearLeftBottom,  nearRightBottom, nearRightTop,  // front face
        farLeftTop,      farLeftBottom,   farRightBottom,  farRightTop,   // far face
        nearLeftBottom,  nearRightBottom, farRightBottom,  farLeftBottom, // bottom face
        nearLeftTop,     nearRightTop,    farRightTop,     farLeftTop,    // top face
        nearLeftBottom,  farLeftBottom,   farLeftTop,      nearLeftTop,   // left face
        nearRightBottom, farRightBottom,  farRightTop,     nearRightTop   // right face
    };

    gizmoProgram->Use();
    gizmoProgram->SetUniform("model", Matrix4x4::Identity());
    gizmoProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
    gizmoProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    gizmoProgram->SetUniform("color", color);
    gizmoBuffer->SubData(vertices, 0, 24 * sizeof(Vector3d));
    Graphics::DrawArrays(*gizmoVao, Primitive::Lines, 0, 24);
    gizmoProgram->UnUse();
  }

  // OpenGL 2.1 version rendering functions implements
  void Graphics::Vertex(const Vector3d& vector) { glVertex3f(vector.x, vector.y, vector.z); }

  void Graphics::Point(const Vector3d& vector, const Color& color, float thickness) {
    SetColor(color);
    glPointSize(thickness);
    glBegin(GL_POINT);
    Vertex(vector);
    glEnd();
  }

  void Graphics::Line(const Vector3d& start, const Vector3d& end, const Color& color,
                      float thickness) {
    glLineWidth(thickness);
    SetColor(color);
    glBegin(GL_LINE_LOOP);
    Vertex(start);
    Vertex(end);
    glEnd();
  }

  void Graphics::Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c,
                          const Color& color, float thickness) {
    Line(a, b, color, thickness);
    Line(b, c, color, thickness);
    Line(c, a, color, thickness);
  }

  void Graphics::SolidTriangle(const Vector3d& a, const Vector3d& b, const Vector3d& c,
                               const Color& color) {
    SetColor(color);
    glBegin(GL_TRIANGLES);
    Vertex(a);
    Vertex(b);
    Vertex(c);
    glEnd();
  }

  void Graphics::Circle(const Vector3d& center, float radius, const Color& color, float thickness) {
    SetColor(color);
    glLineWidth(thickness);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < Math::degrees; i++) {
      const float rad = i * Math::degrees_to_radians;
      Vertex(Vector3d(center.x + Math::Cos(rad) * radius, center.y + Math::Sin(rad) * radius));
    }
    glEnd();
  }

  void Graphics::Quad(const Vector3d& center, float w, float h, const Color& color,
                      float thickness) {
    float halfWidth = w * 0.5f;
    float halfHeight = h * 0.5f;

    Line(Vector3d(-halfWidth + center.x, halfHeight + center.y),
         Vector3d(halfWidth + center.x, halfHeight + center.y), color, thickness);
    Line(Vector3d(halfWidth + center.x, halfHeight + center.y),
         Vector3d(halfWidth + center.x, -halfHeight + center.y), color, thickness);
    Line(Vector3d(halfWidth + center.x, -halfHeight + center.y),
         Vector3d(-halfWidth + center.x, -halfHeight + center.y), color, thickness);
    Line(Vector3d(-halfWidth + center.x, -halfHeight + center.y),
         Vector3d(-halfWidth + center.x, halfHeight + center.y), color, thickness);
  }

  void Graphics::SolidQuad(const Vector3d& center, float w, float h, const Color& color) {
    SetColor(color);

    float halfWidth = w * 0.5f;
    float halfHeight = h * 0.5f;

    glBegin(GL_QUADS);
    Vertex(Vector3d(-halfWidth + center.x, halfHeight + center.y));
    Vertex(Vector3d(halfWidth + center.x, halfHeight + center.y));
    Vertex(Vector3d(halfWidth + center.x, -halfHeight + center.y));
    Vertex(Vector3d(-halfWidth + center.x, -halfHeight + center.y));
    glEnd();
  }

  void Graphics::TexturedQuad(const Vector3d& center, float w, float h, const Vector3d& offset,
                              const Vector3d& scale, const Color& color) {
    SetColor(color);
    float halfWidth = w * 0.5f;
    float halfHeight = h * 0.5f;

    glBegin(GL_QUADS);
    // UL
    glTexCoord2f(offset.x, offset.y);
    Vertex(Vector3d(-halfWidth + center.x, halfHeight + center.y));

    // UR
    glTexCoord2f(offset.x + scale.x, offset.y);
    Vertex(Vector3d(halfWidth + center.x, halfHeight + center.y));

    // LR
    glTexCoord2f(offset.x + scale.x, offset.y + scale.y);
    Vertex(Vector3d(halfWidth + center.x, -halfHeight + center.y));

    // LL
    glTexCoord2f(offset.x, offset.y + scale.y);
    Vertex(Vector3d(-halfWidth + center.x, -halfHeight + center.y));
    glEnd();
  }

  void Graphics::Polygon(const std::vector<Vector3d>& verts, const Color& color, float thickness) {
    glLineWidth(thickness);
    SetColor(color);
    glBegin(GL_LINE_LOOP);
    for (unsigned int i = 0; i < verts.size() - 1; i++) {
      Vertex(verts[i]);
      Vertex(verts[i + 1]);
    }
    glEnd();
  }

  void Graphics::SetDimensions(unsigned int w, unsigned int h) {
    glViewport(0, 0, w, h);
    Set2DGraphics(w, h);
  }

  void Graphics::SetMatrixMode(MatrixMode mode) {
    switch (mode) {
    case MatrixMode::MODELVIEW:
      glMatrixMode(GL_MODELVIEW);
      break;
    case MatrixMode::PROJECTION:
      glMatrixMode(GL_PROJECTION);
      break;
    }
  }

  void Graphics::PopMatrix() { glPopMatrix(); }

  void Graphics::PushMatirx() { glPushMatrix(); }

  void Graphics::Translate(const Vector3d& pos) { glTranslatef(pos.x, pos.y, 0.f); }

  void Graphics::Scale(const Vector3d& scale) { glScalef(scale.x, scale.y, 1.f); }

  void Graphics::Rotate(float r) { glRotatef(r, 0.f, 0.f, 1.f); }

  void Graphics::SetColor(const Color& color) { glColor4f(color.r, color.g, color.b, color.a); }

  void Graphics::Flush() { glFlush(); }

  void Graphics::Set2DGraphics(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, w, h, 0.0, 0.0, 1.0);
  }

  void Graphics::SetBlendMode(BlendMode type) {
    glEnable(GL_BLEND);
    switch (type) {
    case BlendMode::BLEND_ALPHA:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
    case BlendMode::BLEND_ADDITIVE:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      break;
    case BlendMode::BLEND_MULTIPLY:
      glBlendFunc(GL_ZERO, GL_SRC_COLOR);
      break;
    }
  }
}
