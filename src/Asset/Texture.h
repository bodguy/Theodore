// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Texture_h
#define Texture_h

#include "Asset.h"
#include "Graphics/Enumeration.h"
#include "Math/Color.h"
#include "Math/Vector3d.h"
#include "TextureOption.h"

namespace Theodore {
  class Texture : public Asset {
    friend class Debug;
    friend class WaveFrontObjMesh;

  public:
    Texture();
    virtual ~Texture() = 0;

    unsigned char* GetNativeTexturePtr();
    Color GetPixel(unsigned int x, unsigned int y);
    void SetPixel(unsigned int x, unsigned int y, const Color& color);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    unsigned int GetTextureID() const;

    Color GetColorKey() const;
    bool UseColorKey() const;

    void SetMipmapBias(float bias);
    void SetAnisoLevel(float level);
    void SetFilter(const FilterMode mode);
    void SetWrapMode(const WrapMode mode);
    void SetTextureFormat(const TextureFormat format);
    void SetTextureOption(const TextureOption& option);

    TextureDimension GetDimension() const;

  protected:
    unsigned char* nativeTexturePtr;
    unsigned int textureId;
    int width, height;
    float anisoLevel;
    float mipMapBias;

		TextureOption textureOption;
    FilterMode filterMode;
    WrapMode wrapMode;
    TextureDimension textureDimension;
    TextureFormat textureFormat;
    Color colorKey;
    bool useColorKey;
  };
}  // namespace Theodore

#endif /* Texture_h */
