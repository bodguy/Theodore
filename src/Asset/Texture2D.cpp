#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "../Platform/os_types.h"
#include GLEW_INCLUDE_DIR

namespace Theodore {
  Texture2D::Texture2D() {
    mType = AssetType::TextureType;
    mDimension = TextureDimension::Tex2D;
  }

  Texture2D::~Texture2D() {
    stbi_image_free(mNativeTexturePtr);
    glDeleteTextures(1, &mTextureID);
  }

  bool Texture2D::LoadImage(const std::string& filename, TextureFormat format, const Color& colorKey) {
    int w, h, bpp;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
      mColorKey = colorKey;
      mNativeTexturePtr = data;
      mWidth = w;
      mHeight = h;
      SetAssetName(filename);
      SetTextureFormet(format);

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

      glGenTextures(1, &mTextureID);
      glBindTexture(static_cast<GLenum>(mDimension), mTextureID);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
      mFilterMode = FilterMode::Nearest;
      mUseColorKey = true;

      SetMipmapBias(mMipMapBias);
      SetAnisoLevel(mAnisoLevel);
      SetFilter(mFilterMode);
      SetWrapMode(mWrapMode);

      glBindTexture(static_cast<GLenum>(mDimension), static_cast<GLuint>(NULL));

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
      mNativeTexturePtr = data;
      mWidth = w;
      mHeight = h;
      SetAssetName(filename);
      SetTextureFormet(format);

      glGenTextures(1, &mTextureID);
      glBindTexture(static_cast<GLenum>(mDimension), mTextureID);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0);
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
      mFilterMode = FilterMode::Nearest;

      // set parameters
      SetMipmapBias(mMipMapBias);
      SetAnisoLevel(mAnisoLevel);
      SetFilter(mFilterMode);
      SetWrapMode(mWrapMode);

      glBindTexture(static_cast<GLenum>(mDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadRawTextureData(const std::string& filename, TextureFormat format, std::vector<unsigned char>& native, const Color& colorKey) {
    int w, h, bpp;
    unsigned char* data = stbi_load_from_memory(&native.front(), (int)native.size(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
      mColorKey = colorKey;
      mNativeTexturePtr = data;
      mWidth = w;
      mHeight = h;
      SetAssetName(filename);
      SetTextureFormet(format);

      // Go through pixels
      // Size of an image = rows * cols * bpp
      // https://www.tutorialspoint.com/dip/concept_of_bits_per_pixel.htm
      GLuint size = mWidth * mHeight * bpp;
      for (GLuint i = 0; i < size; ++i) {
        // Get pixel colors
        GLubyte* colors = (GLubyte*)&data[i];

        // Color matches
        if (colors[0] == Color::ConvertToByte(mColorKey.r) && colors[1] == Color::ConvertToByte(mColorKey.g) && colors[2] == Color::ConvertToByte(mColorKey.b) &&
            (0 == mColorKey.a || colors[3] == Color::ConvertToByte(mColorKey.a))) {
          // Make transparent
          colors[0] = 000;
          colors[1] = 000;
          colors[2] = 000;
          colors[3] = 000;
        }
      }

      glGenTextures(1, &mTextureID);
      glBindTexture(static_cast<GLenum>(mDimension), mTextureID);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::Red8:
        case TextureFormat::Blue8:
        case TextureFormat::Green8:
        default:
          return false;
      }

      // set parameters
      mFilterMode = FilterMode::Nearest;
      mUseColorKey = true;

      SetMipmapBias(mMipMapBias);
      SetAnisoLevel(mAnisoLevel);
      SetFilter(mFilterMode);
      SetWrapMode(mWrapMode);

      glBindTexture(static_cast<GLenum>(mDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadRawTextureData(const std::string& filename, TextureFormat format, std::vector<unsigned char>& native) {
    int w, h, bpp;
    unsigned char* data = stbi_load_from_memory(&native.front(), (int)native.size(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
      mNativeTexturePtr = data;
      mWidth = (unsigned int)w;
      mHeight = (unsigned int)h;
      SetAssetName(filename);
      SetTextureFormet(format);

      glGenTextures(1, &mTextureID);
      glBindTexture(static_cast<GLenum>(mDimension), mTextureID);
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0);
          glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::Red8:
        case TextureFormat::Blue8:
        case TextureFormat::Green8:
        default:
          return false;
      }
      mFilterMode = FilterMode::Nearest;

      // set parameters
      SetMipmapBias(mMipMapBias);
      SetAnisoLevel(mAnisoLevel);
      SetFilter(mFilterMode);
      SetWrapMode(mWrapMode);

      glBindTexture(static_cast<GLenum>(mDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  bool Texture2D::LoadCustomTexture(unsigned int width, unsigned int height, TextureFormat format, unsigned char* data) {
    glGenTextures(1, &mTextureID);
    glBindTexture(static_cast<GLenum>(mDimension), mTextureID);
    mWidth = width;
    mHeight = height;
    mNativeTexturePtr = data;
    SetTextureFormet(format);

    switch (format) {
      case TextureFormat::RGBA32:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::RGB24:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Red8:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_RED, mWidth, mHeight, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Green8:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_GREEN, mWidth, mHeight, 0, GL_GREEN, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Blue8:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_BLUE, mWidth, mHeight, 0, GL_BLUE, GL_UNSIGNED_BYTE, data);
        break;
      case TextureFormat::Depth:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);
        break;
      case TextureFormat::Stencil:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_STENCIL_INDEX, mWidth, mHeight, 0, GL_STENCIL_INDEX, GL_FLOAT, data);
        break;
      case TextureFormat::Depth24Stencil8:
        glTexImage2D(static_cast<GLenum>(mDimension), 0, GL_DEPTH24_STENCIL8, mWidth, mHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, data);
        break;
      default:
        return false;
    }
    mFilterMode = FilterMode::Nearest;

    SetFilter(mFilterMode);
    SetWrapMode(mWrapMode);
    glBindTexture(static_cast<GLenum>(mDimension), static_cast<GLuint>(NULL));

    return true;
  }
}  // namespace Theodore
