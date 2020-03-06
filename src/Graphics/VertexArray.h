// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef VertexBuffer_h
#define VertexBuffer_h

#include <vector>
#include "Asset/Shader.h"

namespace Theodore {
	class GPUBuffer;
  class VertexArray {
  public:
    VertexArray();
    ~VertexArray();
    VertexArray(const VertexArray& other);
    const VertexArray& operator=(const VertexArray& other);

    unsigned int GetArrayID() const;

    void BindAttribute(const Attribute& attribute, const GPUBuffer& vertexBuffer, unsigned int count, unsigned int stride, unsigned int offset);
    void BindElements(const GPUBuffer& elementBuffer);
    void AttributeDivisor(const Attribute& attribute, unsigned int divisor);

  private:
		unsigned int vertexArrayId;
  };
}  // namespace Theodore

#endif /* VertexBuffer_h */
