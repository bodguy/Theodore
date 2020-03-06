// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "RenderBuffer.h"
#include "Enumeration.h"

namespace Theodore {
  RenderBuffer::RenderBuffer() { glGenRenderbuffers(1, &renderBufferId); }

  RenderBuffer::~RenderBuffer() { glDeleteRenderbuffers(1, &renderBufferId); }

  unsigned int RenderBuffer::GetRenderBufferID() const { return renderBufferId; }

  void RenderBuffer::Storage(unsigned int width, unsigned int height) {
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }
}  // namespace Theodore
