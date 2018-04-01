#include "FrameBuffer.h"
#include "Debug.h"

namespace Quark {
    FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, Enumeration::TextureFormat format) {
        mWidth = width;
        mHeight = height;
        glGenFramebuffers(1, &mFrameBufferID);
        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
        mTextureColor.LoadCustomTexture(width, height, format, NULL);
        mDepthStencil.Storage(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureColor.GetTextureID(), 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencil.GetRenderBufferID());
        glBindFramebuffer(GL_FRAMEBUFFER, NULL);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Debug::Log("Error: Framebuffer is not complete!\n");
        }
    }
    
    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &mFrameBufferID);
    }
    
    unsigned int FrameBuffer::GetFrameBufferID() const {
        return mFrameBufferID;
    }
    
    Texture2D FrameBuffer::GetTexture() const {
        return mTextureColor;
    }
    
    RenderBuffer FrameBuffer::GetDepthStencil() const {
        return mDepthStencil;
    }
    
    unsigned int FrameBuffer::GetWidth() const {
        return mWidth;
    }
    
    unsigned int FrameBuffer::GetHeight() const {
        return mHeight;
    }
}
