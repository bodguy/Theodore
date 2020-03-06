// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include <stb/stb_image.h>
#include "TextureCube.h"
#include "Component/CubemapRenderer.h"

namespace Theodore {
  TextureCube::TextureCube() {
		assetType = AssetType::TextureType;
		textureDimension = TextureDimension::CubeMap;
		wrapMode = WrapMode::Clamp;
		filterMode = FilterMode::Bilinear;
  }

  TextureCube::~TextureCube() { stbi_image_free(nativeTexturePtr); }

  bool TextureCube::LoadCubemapTexture(const CubemapRenderer* cubemap, const std::string& filename, TextureFormat format, CubemapFace face) {
    int w, h, bpp;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, static_cast<int>(format));

    if (data) {
			nativeTexturePtr = data;
			width = w;
			height = h;
      SetAssetName(filename);
      SetTextureFormat(format);

      glBindTexture(static_cast<GLenum>(textureDimension), cubemap->GetTextureID());
      switch (format) {
        case TextureFormat::RGB24:
          glTexImage2D(static_cast<GLenum>(face), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          break;
        case TextureFormat::RGBA32:
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0);
          glTexImage2D(static_cast<GLenum>(face), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
          break;
        default:
          return false;
      }

      // set parameters
      SetFilter(filterMode);
      SetWrapMode(wrapMode);
			cubemapFace = face;

      glBindTexture(static_cast<GLenum>(textureDimension), static_cast<GLuint>(NULL));

      return true;
    }

    return false;
  }

  CubemapFace TextureCube::GetFace() const { return cubemapFace; }
}  // namespace Theodore
