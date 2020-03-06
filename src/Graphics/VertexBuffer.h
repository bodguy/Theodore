// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef VertexBuffer_h
#define VertexBuffer_h

#include "Asset/Shader.h"
#include "Enumeration.h"
#include "Platform/CondiitonalVars.h"
#include GLEW_INCLUDE_DIR
#include <vector>

namespace Theodore {
  class Vector2d;
  class Vector3d;
  class Vector4d;
  class Color;
  class InputStream {
  public:
    void Float(float v);
    void Int8(char v);
    void Int16(short v);
    void Int32(int v);
    void Uint8(unsigned char v);
    void Uint16(unsigned short v);
    void Uint32(unsigned int v);

    void Vec2(const Vector2d& v);
    void Vec3(const Vector3d& v);
    void Vec4(const Vector4d& v);
    void Color4(const Color& v);

    void* Pointer();
    unsigned long Size();

  private:
    std::vector<unsigned char> buffer;

    void Bytes(unsigned char* bytes, unsigned int count);
  };

  class Buffer {
  public:
    Buffer(BufferType type);
    Buffer(const void* data, size_t length, BufferUsage usage, BufferType type);
    ~Buffer();
    Buffer(const Buffer& other);

    const Buffer& operator=(Buffer other);
    unsigned int GetBufferID() const;

    void Data(const void* data, size_t length, BufferUsage usage);
    // this redefines some or all of the data store for the specified buffer object.
    void SubData(const void* data, size_t offset, size_t length);
    void GetSubData(void* data, size_t offset, size_t length);
    void BindBase(unsigned int index);
    // map a portion of the buffer.
    void BindRange(unsigned int index, unsigned int offset, unsigned int size);
    // map the whole buffer
    void* Lock(LockMode mode);
    void UnLock();

  private:
    GLuint wertexBufferId;
    BufferType bufferType;
    size_t length;
  };

  class VertexArray {
  public:
    VertexArray();
    ~VertexArray();
    VertexArray(const VertexArray& other);
    const VertexArray& operator=(const VertexArray& other);

    unsigned int GetArrayID() const;

    void BindAttribute(const Attribute& attribute, const Buffer& buffer, unsigned int count, unsigned int stride, unsigned int offset);
    void BindElements(const Buffer& elements);
    void AttributeDivisor(const Attribute& attribute, unsigned int divisor);

  private:
    GLuint vertexArrayId;
  };
}  // namespace Theodore

#endif /* VertexBuffer_h */
