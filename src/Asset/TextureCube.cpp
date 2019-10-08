// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "TextureCube.h"
#include "Object/Component/CubemapRenderer.h"
#include <stb/stb_image.h>

namespace Theodore {
  TextureCube::TextureCube() {
    mType = AssetType::TextureType;
    mDimension = TextureDimension::CubeMap;
    mWrapMode = WrapMode::Clamp;
    mFilterMode = FilterMode::Bilinear;
  }

  TextureCube::~TextureCube() { stbi_image_free(mNativeTexturePtr); }

  bool TextureCube::LoadCubemapTexture(const CubemapRenderer* cubemap, const std::string& filename, TextureFormat format, CubemapFace face) {
    int w, h, bpp;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
      mNativeTexturePtr = data;
      mWidth = w;
      mHeight = h;
      SetAssetName(filename);
      SetTextureFormet(format);

      glBindTexture(static_cast<GLenum>(mDimension), cubemap->GetTextureID());
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(face), 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0);
          glTexImage2D(static_cast<GLenum>(face), 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        default:
          return false;
      }

      // set parameters
      SetFilter(mFilterMode);
      SetWrapMode(mWrapMode);
      mFace = face;

      glBindTexture(static_cast<GLenum>(mDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  CubemapFace TextureCube::GetFace() const { return mFace; }
}  // namespace Theodore
