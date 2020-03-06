// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "VertexBuffer.h"

#include "Math/Color.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"
#include "Math/Vector4d.h"

namespace Theodore {

  ////////////////////////////////////////////////////////////////////////////////////
  // InputStream

  void InputStream::Float(float v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Int8(char v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Int16(short v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Int32(int v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Uint8(unsigned char v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Uint16(unsigned short v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Uint32(unsigned int v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Vec2(const Vector2d& v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Vec3(const Vector3d& v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Vec4(const Vector4d& v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void InputStream::Color4(const Color& v) { Bytes((unsigned char*)&v, sizeof(v)); }

  void* InputStream::Pointer() { return &buffer[0]; }

  unsigned long InputStream::Size() { return buffer.size(); }

  void InputStream::Bytes(unsigned char* bytes, unsigned int count) {
    for (unsigned int i = 0; i < count; i++) buffer.push_back(bytes[i]);
  }

  ////////////////////////////////////////////////////////////////////////////////////
  // Buffer

  Buffer::Buffer(BufferType type) {
		bufferType = type;
		length = 0;
    glGenBuffers(1, &wertexBufferId);
  }

  Buffer::Buffer(const void* data, size_t length, BufferUsage usage, BufferType type) {
    glGenBuffers(1, &wertexBufferId);
		bufferType = type;
    Data(data, length, usage);
  }

  Buffer::~Buffer() { glDeleteBuffers(1, &wertexBufferId); }

  Buffer::Buffer(const Buffer& other) {
    if (length) {
      glBindBuffer(GL_COPY_READ_BUFFER, other.wertexBufferId);
      glBindBuffer(GL_COPY_WRITE_BUFFER, wertexBufferId);
      glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, length);
    }

		wertexBufferId = other.wertexBufferId;
		bufferType = other.bufferType;
		length = other.length;
  }

  const Buffer& Buffer::operator=(Buffer other) {
    if (length) {
      glBindBuffer(GL_COPY_READ_BUFFER, other.wertexBufferId);
      glBindBuffer(GL_COPY_WRITE_BUFFER, wertexBufferId);
      glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, length);
    }

		wertexBufferId = other.wertexBufferId;
		bufferType = other.bufferType;
		length = other.length;
    return *this;
  }

  unsigned int Buffer::GetBufferID() const { return wertexBufferId; }

  void Buffer::Data(const void* data, size_t length, BufferUsage usage) {
		length = length;
    glBindBuffer(static_cast<GLenum>(bufferType), wertexBufferId);
    glBufferData(static_cast<GLenum>(bufferType), length, data, static_cast<GLenum>(usage));
  }

  void Buffer::SubData(const void* data, size_t offset, size_t length) {
    glBindBuffer(static_cast<GLenum>(bufferType), wertexBufferId);
    glBufferSubData(static_cast<GLenum>(bufferType), offset, length, data);
  }

  void Buffer::GetSubData(void* data, size_t offset, size_t length) {
    glBindBuffer(static_cast<GLenum>(bufferType), wertexBufferId);
    glGetBufferSubData(static_cast<GLenum>(bufferType), offset, length, data);
  }

  void Buffer::BindBase(unsigned int index) { glBindBufferBase(static_cast<GLenum>(bufferType), index, wertexBufferId); }

  void Buffer::BindRange(unsigned int index, unsigned int offset, unsigned int size) { glBindBufferRange(static_cast<GLenum>(bufferType), index, wertexBufferId, offset, size); }

  void* Buffer::Lock(LockMode mode) { return glMapBuffer(static_cast<GLenum>(bufferType), static_cast<GLenum>(mode)); }

  void Buffer::UnLock() { glUnmapBuffer(static_cast<GLenum>(bufferType)); }

  ////////////////////////////////////////////////////////////////////////////////////
  // VertexArray

  VertexArray::VertexArray() { glGenVertexArrays(1, &vertexArrayId); }

  VertexArray::VertexArray(const VertexArray& other) { vertexArrayId = other.vertexArrayId; }

  VertexArray::~VertexArray() { glDeleteVertexArrays(1, &vertexArrayId); }

  unsigned int VertexArray::GetArrayID() const { return vertexArrayId; }

  const VertexArray& VertexArray::operator=(const VertexArray& other) {
		vertexArrayId = other.vertexArrayId;
    return *this;
  }

  void VertexArray::BindAttribute(const Attribute& attribute, const Buffer& buffer, unsigned int count, unsigned int stride, unsigned int offset) {
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.GetBufferID());
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, count, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
    glBindVertexArray(NULL);
  }

  void VertexArray::BindElements(const Buffer& elements) {
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements.GetBufferID());
    glBindVertexArray(NULL);
  }

  void VertexArray::AttributeDivisor(const Attribute& attribute, unsigned int divisor) {
    glBindVertexArray(vertexArrayId);
    glVertexAttribDivisor(attribute, divisor);
    glBindVertexArray(NULL);
  }
}  // namespace Theodore
