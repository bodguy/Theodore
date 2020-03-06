// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "VertexArray.h"
#include "GPUBuffer.h"

namespace Theodore {
  VertexArray::VertexArray() { glGenVertexArrays(1, &vertexArrayId); }

  VertexArray::VertexArray(const VertexArray& other) { vertexArrayId = other.vertexArrayId; }

  VertexArray::~VertexArray() { glDeleteVertexArrays(1, &vertexArrayId); }

  unsigned int VertexArray::GetArrayID() const { return vertexArrayId; }

  const VertexArray& VertexArray::operator=(const VertexArray& other) {
		vertexArrayId = other.vertexArrayId;
    return *this;
  }

  void VertexArray::BindAttribute(const Attribute& attribute, const GPUBuffer& vertexBuffer, unsigned int count, unsigned int stride, unsigned int offset) {
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.GetBufferID());
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, count, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset));
    glBindVertexArray(NULL);
  }

  void VertexArray::BindElements(const GPUBuffer& elementBuffer) {
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.GetBufferID());
    glBindVertexArray(NULL);
  }

  void VertexArray::AttributeDivisor(const Attribute& attribute, unsigned int divisor) {
    glBindVertexArray(vertexArrayId);
    glVertexAttribDivisor(attribute, divisor);
    glBindVertexArray(NULL);
  }
}  // namespace Theodore
