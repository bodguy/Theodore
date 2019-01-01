#include "FrameBuffer.h"
#include "Debug.h"
#include "RenderBuffer.h"
#include "Texture2D.h"

namespace Theodore {
  FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
      : mWidth(width), mHeight(height), mRender(nullptr), mIsCreated(false) {
    mTextures.clear();
  }

  FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &mFrameBufferID); }

  bool FrameBuffer::AttachTexture(Texture2D* tex, Attachment attach) {
    return mTextures.insert(std::make_pair(attach, tex)).second;
  }

  Texture2D* FrameBuffer::GetRenderTexture(Attachment attach) { return mTextures[attach]; }

  void FrameBuffer::SetRenderBuffer(RenderBuffer* buffer) { mRender = buffer; }

  bool FrameBuffer::Create(bool writable) {
    glGenFramebuffers(1, &mFrameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
    for (std::pair<Attachment, Texture2D*> p : mTextures) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(p.first),
                             static_cast<int>(p.second->GetDimension()), p.second->GetTextureID(),
                             0);
    }

    if (mRender) {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                mRender->GetRenderBufferID());
    }

    if (!writable) {
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);

    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (e != GL_FRAMEBUFFER_COMPLETE) {
      switch (e) {
      case GL_FRAMEBUFFER_UNDEFINED:
        Debug::Log("Error: Framebuffer Undefined");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        Debug::Log("Error: Framebuffer Incomplete Attachment");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        Debug::Log("Error: Framebuffer Missing Attachment");
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        Debug::Log("Error: Framebuffer Incomplete Draw Buffer");
        break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
        Debug::Log("Error: Framebuffer Unsupported");
        break;
      default:
        Debug::Log("Error: Framebuffer Problem");
      }
      return false;
    }

    Debug::Log("FrameBuffer successfully created [id=%d]", mFrameBufferID);
    mIsCreated = true;
    return true;
  }

  bool FrameBuffer::IsCreated() const { return mIsCreated; }

  unsigned int FrameBuffer::GetFrameBufferID() const { return mFrameBufferID; }

  unsigned int FrameBuffer::GetWidth() const { return mWidth; }

  unsigned int FrameBuffer::GetHeight() const { return mHeight; }

  Texture2D* FrameBuffer::GetTexture(Attachment attach) { return mTextures[attach]; }

  void FrameBuffer::GetBufferInfo() {
    /*
      glGetFramebufferAttachmentParameteriv();
      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE: returns the type of object which contains either
      GL_RENDERBUFFER, GL_TEXTURE, GL_NONE.
      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME: returns the name of object
      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL: mipmap level of the texture (only in GL_TEXTURE)
      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE: initial value is
      GL_TEXTURE_CUBE_MAP_POSITIVE_X
    */
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

    int res, i = 0;
    GLint buffer;

    do {
      glGetIntegerv(GL_DRAW_BUFFER0 + i, &buffer);
      if (buffer != GL_NONE) {
        Debug::Log("Shader Output Location %d - color attachment %d", i,
                   buffer - GL_COLOR_ATTACHMENT0);
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, buffer,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
        Debug::Log("\tAttachment Type: %s", res == GL_TEXTURE ? "Texture" : "Render Buffer");
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, buffer,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
        Debug::Log("\tAttachment object name: %d", res);
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, buffer,
                                              GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL, &res);
        Debug::Log("\tAttachment object level: %d", res);
      }
      ++i;
    } while (buffer != GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
  }

  void FrameBuffer::SetDimension(unsigned int w, unsigned int h) {
    mWidth = w;
    mHeight = h;
  }
}
