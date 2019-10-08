// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "RenderBuffer.h"
#include "Platform/os_types.h"
#include GLEW_INCLUDE_DIR

namespace Theodore {
  RenderBuffer::RenderBuffer() { glGenRenderbuffers(1, &mRenderBufferID); }

  RenderBuffer::~RenderBuffer() { glDeleteRenderbuffers(1, &mRenderBufferID); }

  unsigned int RenderBuffer::GetRenderBufferID() const { return mRenderBufferID; }

  void RenderBuffer::Storage(unsigned int width, unsigned int height) {
    glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }
}  // namespace Theodore
