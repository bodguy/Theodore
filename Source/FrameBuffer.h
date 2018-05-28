#ifndef FrameBuffer_h
#define FrameBuffer_h

#include "Enumeration.h"
#include <map>

namespace Quark {
	class Texture2D; class RenderBuffer;
    class FrameBuffer {
    public:
        FrameBuffer(unsigned int width, unsigned int height);
        ~FrameBuffer();

		bool Create(bool writable);

		bool AttachTexture(Texture2D* tex, Attachment attach);
		void SetRenderBuffer(RenderBuffer* buffer);
        
        unsigned int GetFrameBufferID() const;
        unsigned int GetWidth() const;
        unsigned int GetHeight() const;
		Texture2D* GetTexture(Attachment attach);

		void SetDimension(unsigned int w, unsigned int h);

    private:
        unsigned int mFrameBufferID;
        unsigned int mWidth;
        unsigned int mHeight;
		std::map<Attachment, Texture2D*> mTextures;
		RenderBuffer* mRender;
    };
}

#endif /* FrameBuffer_h */
