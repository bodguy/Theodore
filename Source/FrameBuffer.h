#ifndef FrameBuffer_h
#define FrameBuffer_h

#include "Enumeration.h"
#include "Texture2D.h"
#include "RenderBuffer.h"

namespace Quark {
    class FrameBuffer {
    public:
        FrameBuffer(unsigned int width, unsigned int height, TextureFormat format);
        ~FrameBuffer();
        
        unsigned int GetFrameBufferID() const;
        Texture2D GetTexture() const;
        RenderBuffer GetDepthStencil() const;
        
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
    private:
        unsigned int mFrameBufferID;
        Texture2D mTextureColor;
        RenderBuffer mDepthStencil;
        unsigned int mWidth;
        unsigned int mHeight;
    };
}

#endif /* FrameBuffer_h */
