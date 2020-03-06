// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Texture2D.h"

#include "Platform/CondiitonalVars.h"
#include GLEW_INCLUDE_DIR
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Theodore {
  Texture2D::Texture2D() {
		assetType = AssetType::TextureType;
		textureDimension = TextureDimension::Tex2D;
  }

  Texture2D::~Texture2D() {
    stbi_image_free(nativeTexturePtr);
    glDeleteTextures(1, &textureId);
  }

  bool Texture2D::LoadImage(const std::string& filename, TextureFormat format, const Color& colorKey) {
    int w, h, bpp;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
			colorKey = colorKey;
			nativeTexturePtr = data;
			width = w;
			height = h;
      SetAssetName(filename);
      SetTextureFormat(format);

      //	 NOTE: this codes are very old-fashioned opengl stuff, so DO NOT USE otherwise,
      //        it will considerably corrode your application performances at loading time.
      //		   I recommend to use discard idiom in fragment shader.

      //       // Go through pixels
      //       // Size of an image = rows * cols * bpp
      //       // https://www.tutorialspoint.com/dip/concept_of_bits_per_pixel.htm
      //       GLuint size = mWidth * mHeight * bpp;
      //       for (GLuint i = 0; i < size; ++i) {
      //           //Get pixel colors
      //           GLubyte* colors = (GLubyte*)&data[i];
      //
      //           //Color matches
      //           if (colors[0] == Color::ConvertToByte(mColorKey.r) &&
      //               colors[1] == Color::ConvertToByte(mColorKey.g) &&
      //               colors[2] == Color::ConvertToByte(mColorKey.b) &&
      //(0 == mColorKey.a || colors[3] == Color::ConvertToByte(mColorKey.a)) ) {
      //               //Make transparent
      //               colors[0] = 000;
      //               colors[1] = 000;
      //               colors[2] = 000;
      //               colors[3] = 000;
      //           }
      //       }

      glGenTextures(1, &textureId);
      glBindTexture(static_cast<GLenum>(textureDimension), textureId);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::Red8:
        case TextureFormat::Blue8:
        case TextureFormat::Green8:
          break;
        default:
          return false;
      }

      // set parameters
      // https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/discard.php
      filterMode = FilterMode::Nearest;
			useColorKey = true;

      SetMipmapBias(mipMapBias);
      SetAnisoLevel(anisoLevel);
      SetFilter(filterMode);
      SetWrapMode(wrapMode);

      glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadImage(const std::string& filename, TextureFormat format) {
    int w, h, bpp;

    //		if (format == TextureFormat::sRGB24) {
    //#define STBI_NO_HDR
    //			stbi_ldr_to_hdr_scale(1.0f);
    //			stbi_ldr_to_hdr_gamma(2.2f);
    //			format = TextureFormat::RGB24;
    //		}
    //
    //		if (format == TextureFormat::sRGBA32) {
    //#define STBI_NO_HDR
    //			stbi_ldr_to_hdr_scale(1.0f);
    //			stbi_ldr_to_hdr_gamma(2.2f);
    //			format = TextureFormat::RGBA32;
    //		}

    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
			nativeTexturePtr = data;
			width = w;
			height = h;
      SetAssetName(filename);
      SetTextureFormat(format);

      glGenTextures(1, &textureId);
      glBindTexture(static_cast<GLenum>(textureDimension), textureId);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0);
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::Red8:
        case TextureFormat::Blue8:
        case TextureFormat::Green8:
        // case TextureFormat::sRGB24:
        //	glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_SRGB, mWidth, mHeight, 0, GL_RGB,
        // GL_UNSIGNED_BYTE, data);
        //	break;
        // case TextureFormat::sRGBA32:
        //	glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_SRGB_ALPHA, mWidth, mHeight, 0,
        // GL_RGBA, GL_UNSIGNED_BYTE, data);
        //	break;
        default:
          return false;
      }
			filterMode = FilterMode::Nearest;

      // set parameters
      SetMipmapBias(mipMapBias);
      SetAnisoLevel(anisoLevel);
      SetFilter(filterMode);
      SetWrapMode(wrapMode);

      glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadRawTextureData(const std::string& filename, TextureFormat format, std::vector<unsigned char>& native, const Color& colorKey) {
    int w, h, bpp;
    unsigned char* data = stbi_load_from_memory(&native.front(), (int)native.size(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
			colorKey = colorKey;
			nativeTexturePtr = data;
			width = w;
			height = h;
      SetAssetName(filename);
      SetTextureFormat(format);

      // Go through pixels
      // Size of an image = rows * cols * bpp
      // https://www.tutorialspoint.com/dip/concept_of_bits_per_pixel.htm
      GLuint size = width * height * bpp;
      for (GLuint i = 0; i < size; ++i) {
        // Get pixel colors
        GLubyte* colors = (GLubyte*)&data[i];

        // clang-format off
        // Color matches
        if (colors[0] == Color::ToByte(colorKey.r) &&
            colors[1] == Color::ToByte(colorKey.g) &&
            colors[2] == Color::ToByte(colorKey.b) &&
						(0 == colorKey.a || colors[3] == Color::ToByte(colorKey.a))
        ) {
          // Make transparent
          colors[0] = 000;
          colors[1] = 000;
          colors[2] = 000;
          colors[3] = 000;
        }
        // clang-format on
      }

      glGenTextures(1, &textureId);
      glBindTexture(static_cast<GLenum>(textureDimension), textureId);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::Red8:
        case TextureFormat::Blue8:
        case TextureFormat::Green8:
        default:
          return false;
      }

      // set parameters
      filterMode = FilterMode::Nearest;
			useColorKey = true;

      SetMipmapBias(mipMapBias);
      SetAnisoLevel(anisoLevel);
      SetFilter(filterMode);
      SetWrapMode(wrapMode);

      glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadRawTextureData(const std::string& filename, TextureFormat format, std::vector<unsigned char>& native) {
    int w, h, bpp;
    unsigned char* data = stbi_load_from_memory(&native.front(), (int)native.size(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
			nativeTexturePtr = data;
			width = (unsigned int)w;
			height = (unsigned int)h;
      SetAssetName(filename);
      SetTextureFormat(format);

      glGenTextures(1, &textureId);
      glBindTexture(static_cast<GLenum>(textureDimension), textureId);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0);
          glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::Red8:
        case TextureFormat::Blue8:
        case TextureFormat::Green8:
        default:
          return false;
      }
			filterMode = FilterMode::Nearest;

      // set parameters
      SetMipmapBias(mipMapBias);
      SetAnisoLevel(anisoLevel);
      SetFilter(filterMode);
      SetWrapMode(wrapMode);

      glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadCustomTexture(unsigned int width, unsigned int height, TextureFormat format, unsigned char* data) {
    glGenTextures(1, &textureId);
    glBindTexture(static_cast<GLenum>(textureDimension), textureId);
		width = width;
		height = height;
		nativeTexturePtr = data;
    SetTextureFormat(format);

    switch (format) {
      case TextureFormat::RGBA32:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::RGB24:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Red8:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Green8:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_GREEN, width, height, 0, GL_GREEN, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Blue8:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_BLUE, width, height, 0, GL_BLUE, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Depth:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
        break;
      case TextureFormat::Stencil:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_FLOAT, data);
        break;
      case TextureFormat::Depth24Stencil8:
        glTexImage2D(static_cast<GLenum>(textureDimension), 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, data);
        break;
      default:
        return false;
    }
		filterMode = FilterMode::Nearest;

    SetFilter(filterMode);
    SetWrapMode(wrapMode);
    glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

    return true;
  }
}  // namespace Theodore
