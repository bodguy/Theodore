#include "TextureCube.h"
#include <stb/stb_image.h>

namespace Quark {
	TextureCube::TextureCube() {
		mType = Enumeration::TextureType;
		mDimension = Enumeration::CubeMap;
		mWrapMode = Enumeration::Clamp;
		mFilterMode = Enumeration::Bilinear;
	}

	TextureCube::~TextureCube() {
		stbi_image_free(mNativeTexturePtr);
	}

	bool TextureCube::LoadCubemapTexture(unsigned int id, const std::string& filename, Enumeration::TextureFormat format, Enumeration::CubemapFace face) {
		int w, h, bpp;
		unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, format);

		if (data) {
			mNativeTexturePtr = data;
			mWidth = w;
			mHeight = h;
			mName = filename;

			glBindTexture(mDimension, id);
			switch (format) {
			case Enumeration::RGB24:
				glTexImage2D(face, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				break;
			case Enumeration::RGBA32:
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0);
				glTexImage2D(face, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				break;
            case Enumeration::Red8:
            case Enumeration::Blue8:
            case Enumeration::Green8:
                break;
			}

			// set parameters
			SetFilter(mFilterMode);
			SetWrapMode(mWrapMode);

			glBindTexture(mDimension, NULL);

			return true;
		}

		return false;
	}
}
