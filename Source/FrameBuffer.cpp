#include "FrameBuffer.h"
#include "Debug.h"
#include "Texture2D.h"
#include "RenderBuffer.h"

namespace Quark {
    FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) :mWidth(width), mHeight(height), mRender(nullptr) {
		mTextures.clear();
    }
    
    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &mFrameBufferID);
    }

	bool FrameBuffer::AttachTexture(Texture2D* tex, Attachment attach) {
		return mTextures.insert(std::make_pair(attach, tex)).second;
	}

	void FrameBuffer::SetRenderBuffer(RenderBuffer* buffer) {
		mRender = buffer;
	}

	bool FrameBuffer::Create(bool writable) {
		glGenFramebuffers(1, &mFrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
		for (std::pair<Attachment, Texture2D*> p : mTextures) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(p.first), static_cast<int>(p.second->GetDimension()), p.second->GetTextureID(), 0);
		}

		if (mRender) {
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRender->GetRenderBufferID());
		}

		if (!writable) {
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			Debug::Log("Error: Framebuffer is not complete!");
			return false;
		}

		Debug::Log("FrameBuffer successfully created [id=%d]", mFrameBufferID);

		return true;
	}
    
    unsigned int FrameBuffer::GetFrameBufferID() const {
        return mFrameBufferID;
    }
    
    unsigned int FrameBuffer::GetWidth() const {
        return mWidth;
    }
    
    unsigned int FrameBuffer::GetHeight() const {
        return mHeight;
    }

	Texture2D* FrameBuffer::GetTexture(Attachment attach) {
		return mTextures[attach];
	}

	void FrameBuffer::SetDimension(unsigned int w, unsigned int h) {
		mWidth = w;
		mHeight = h;
	}
}
