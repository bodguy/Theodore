// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Texture.h"

namespace Theodore {
  Texture::Texture()
      : nativeTexturePtr(nullptr),
				textureId(1),
				width(0),
				height(0),
				mipMapBias(1000.f),
				textureOption(),
				filterMode(FilterMode::Trilinear),
				wrapMode(WrapMode::ClampEdge),
				textureDimension(TextureDimension::None),
				textureFormat(TextureFormat::UNKNOWN),
				colorKey(Color::white),
				useColorKey(false) {
		assetType = AssetType::TextureType;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoLevel);
  }

  Texture::~Texture() {}

  unsigned char* Texture::GetNativeTexturePtr() { return nativeTexturePtr; }

  Color Texture::GetPixel(unsigned int x, unsigned int y) {
    GLubyte* colors = (GLubyte*)&nativeTexturePtr[y * width + x];
    return Color::FromRGBA(colors[0], colors[1], colors[2], colors[3] ? colors[3] : 0);
  }

  void Texture::SetPixel(unsigned int x, unsigned int y, const Color& color) {
    GLubyte* colors = (GLubyte*)&nativeTexturePtr[y * width + x];
    colors[0] = Color::ToByte(color.r);  // r
    colors[1] = Color::ToByte(color.g);  // g
    colors[2] = Color::ToByte(color.b);  // b
    colors[3] = Color::ToByte(color.a);  // a
  }

  unsigned int Texture::GetWidth() const { return width; }

  unsigned int Texture::GetHeight() const { return height; }

  unsigned int Texture::GetTextureID() const { return textureId; }

  Color Texture::GetColorKey() const { return colorKey; }

  bool Texture::UseColorKey() const { return useColorKey; }

  void Texture::SetMipmapBias(float bias) {
		mipMapBias = bias;
    // Set mipmap bias value
    if (filterMode == FilterMode::Trilinear) {
      glTexParameterf(GL_TEXTURE_ENV, GL_TEXTURE_MIN_LOD, -mipMapBias);
      glTexParameterf(GL_TEXTURE_ENV, GL_TEXTURE_MAX_LOD, mipMapBias);
    }
  }

  void Texture::SetAnisoLevel(float level) {
		anisoLevel = level;
    // Set anisotrophy filtering
    glTexParameterf(static_cast<GLenum>(textureDimension), GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoLevel);
  }

  void Texture::SetFilter(const FilterMode mode) {
		filterMode = mode;
    // Set filtering mode
    glTexParameteri(static_cast<GLenum>(textureDimension), GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(filterMode));
    glTexParameteri(static_cast<GLenum>(textureDimension), GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(filterMode));
    if (filterMode == FilterMode::Trilinear) glGenerateMipmap(static_cast<GLenum>(textureDimension));
  }

  void Texture::SetWrapMode(const WrapMode mode) {
		wrapMode = mode;
    // Set wrap mode
    glTexParameteri(static_cast<GLenum>(textureDimension), GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrapMode));
    glTexParameteri(static_cast<GLenum>(textureDimension), GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrapMode));
    if (textureDimension == TextureDimension::CubeMap) glTexParameteri(static_cast<GLenum>(textureDimension), GL_TEXTURE_WRAP_R, static_cast<GLenum>(wrapMode));
  }

  void Texture::SetTextureFormat(const TextureFormat format) { textureFormat = format; }

	void Texture::SetTextureOption(const TextureOption& option) {
		textureOption = option;
  }

  TextureDimension Texture::GetDimension() const { return textureDimension; }
}  // namespace Theodore
