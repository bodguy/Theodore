#include "Graphics.h"
#include "Color.h"
#include "VertexBuffer.h"
#include "FrameBuffer.h"
#include "Debug.h"

namespace Quark {
    void Graphics::Clear(const Color& color, Enumeration::BufferBits bits) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(bits);
    }
    
    void Graphics::BindTexture(unsigned int unit, Texture* texture) {
        glActiveTexture(GL_TEXTURE0 + unit);
        if (texture) {
            glBindTexture(texture->GetDimension(), texture->GetTextureID());
        } else {
            glBindTexture(GL_TEXTURE_2D, NULL);
        }
    }
    
    void Graphics::BindFrameBuffer(FrameBuffer* buffer) {
        if (buffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, buffer->GetFrameBufferID());
            glViewport(0, 0, buffer->GetWidth(), buffer->GetHeight());
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, NULL);
        }
    }
    
    void Graphics::DrawArrays(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int vertices) {
        glBindVertexArray(vao.GetArrayID());
        glDrawArrays(mode, offset, vertices);
        glBindVertexArray(0);
    }
    
    void Graphics::DrawElements(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int count, unsigned int type) {
        glBindVertexArray(vao.GetArrayID());
        glDrawElements(mode, count, type, reinterpret_cast<const void*>(offset));
        glBindVertexArray(0);
    }
    
    void Graphics::DrawArraysInstanced(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int vertices, unsigned int count) {
        glBindVertexArray(vao.GetArrayID());
        glDrawArraysInstanced(mode, offset, vertices, count);
        glBindVertexArray(0);
    }
    
    void Graphics::DrawElementsInstanced(const VertexArray& vao, Enumeration::Primitive mode, unsigned int offset, unsigned int count, unsigned int type, unsigned int primcount) {
        glBindVertexArray(vao.GetArrayID());
        glDrawElementsInstanced(mode, count, type, reinterpret_cast<const void*>(offset), primcount);
        glBindVertexArray(0);
    }
    
    void Graphics::GetLastError() {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            switch (err) {
                case GL_INVALID_ENUM: // 0x0500
                    Debug::Log("GL_INVALID_ENUM\n");
                    break;
                case GL_INVALID_VALUE: // 0x0501
                    Debug::Log("GL_INVALID_VALUE\n");
                    break;
                case GL_INVALID_OPERATION: // 0x0502
                    Debug::Log("GL_INVALID_OPERATION\n");
                    break;
                case GL_STACK_OVERFLOW: // 0x0503
                    Debug::Log("GL_STACK_OVERFLOW\n");
                    break;
                case GL_STACK_UNDERFLOW: // 0x0504
                    Debug::Log("GL_STACK_UNDERFLOW\n");
                    break;
                case GL_OUT_OF_MEMORY: // 0x0505
                    Debug::Log("GL_OUT_OF_MEMORY\n");
                    break;
            }
        }
    }
    
    void Graphics::SetPolygonMode(Enumeration::FillMode value) {
        glPolygonMode( GL_FRONT_AND_BACK, value );
    }
    
    void Graphics::Enable(Enumeration::Capabilities value) {
        glEnable(value);
    }
    
    void Graphics::Disable(Enumeration::Capabilities value) {
        glDisable(value);
    }
}
