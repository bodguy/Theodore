// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Texture_h
#define Texture_h

#include "Asset.h"
#include "Graphics/Enumeration.h"
#include "Math/Color.h"
#include "Platform/os_types.h"
#include GLEW_INCLUDE_DIR
#include <vector>

namespace Theodore {
  class Texture : public Asset {
    friend class Debug;

  public:
    Texture();
    virtual ~Texture() = 0;

    unsigned char* GetNativeTexturePtr();
    Color GetPixel(unsigned int x, unsigned int y);
    void SetPixel(unsigned int x, unsigned int y, const Color& color);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    unsigned int GetTextureID() const;
    std::string GetMappingType() const;

    Color GetColorKey() const;
    bool UseColorKey() const;

    void SetMipmapBias(float bias);
    void SetAnisoLevel(float level);
    void SetFilter(const FilterMode mode);
    void SetWrapMode(const WrapMode mode);
    void SetMappingType(const std::string& type);
    void SetTextureFormat(const TextureFormat format);

    TextureDimension GetDimension() const;

  protected:
    unsigned char* mNativeTexturePtr;
    unsigned int mTextureID;
    int mWidth, mHeight;
    float mAnisoLevel;
    float mMipMapBias;
    std::string mMappingType;

    FilterMode mFilterMode;
    WrapMode mWrapMode;
    TextureDimension mDimension;
    TextureFormat mTextureFormat;
    Color mColorKey;
    bool mUseColorKey;
  };
}  // namespace Theodore

#endif /* Texture_h */
