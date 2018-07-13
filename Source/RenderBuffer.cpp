#include "RenderBuffer.h"
#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Theodore {
    RenderBuffer::RenderBuffer() {
        glGenRenderbuffers(1, &mRenderBufferID);
    }
    
    RenderBuffer::~RenderBuffer() {
        glDeleteRenderbuffers(1, &mRenderBufferID);
    }
    
    unsigned int RenderBuffer::GetRenderBufferID() const {
        return mRenderBufferID;
    }
    
    void RenderBuffer::Storage(unsigned int width, unsigned int height) {
        glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}
