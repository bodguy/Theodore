#include "MSAATexture2D.h"

namespace Quark {
	MSAATexture2D::MSAATexture2D() {
		mType = Enumeration::TextureType;
		mDimension = Enumeration::MSAATex2D;
	}

	MSAATexture2D::~MSAATexture2D() {
	}

	bool MSAATexture2D::LoadMultiSampleTexture(unsigned int width, unsigned int height, Enumeration::TextureFormat format, unsigned int sample) {
		glGenTextures(1, &mTextureID);
		glBindTexture(mDimension, mTextureID);
		mWidth = width;
		mHeight = height;

		switch (format) {
		case Enumeration::RGBA32:
			glTexImage2DMultisample(mDimension, sample, GL_RGBA8, mWidth, mHeight, GL_TRUE);
			break;
		case Enumeration::RGB24:
			glTexImage2DMultisample(mDimension, sample, GL_RGB, mWidth, mHeight, GL_TRUE);
			break;
		case Enumeration::Red8:
			glTexImage2DMultisample(mDimension, sample, GL_RED, mWidth, mHeight, GL_TRUE);
			break;
		case Enumeration::Green8:
			glTexImage2DMultisample(mDimension, sample, GL_GREEN, mWidth, mHeight, GL_TRUE);
			break;
		case Enumeration::Blue8:
			glTexImage2DMultisample(mDimension, sample, GL_BLUE, mWidth, mHeight, GL_TRUE);
			break;
		}

		glBindTexture(mDimension, NULL);

		return true;
	}
}
