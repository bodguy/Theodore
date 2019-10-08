// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Texture2D_h
#define Texture2D_h

#include "Graphics/Enumeration.h"
#include "Texture.h"

namespace Theodore {
  class Texture2D : public Texture {
  public:
    Texture2D();
    virtual ~Texture2D() override;

    bool LoadImage(const std::string& filename, TextureFormat format, const Color& colorKey);
    bool LoadImage(const std::string& filename, TextureFormat format);
    bool LoadRawTextureData(const std::string& filename, TextureFormat format, std::vector<unsigned char>& native, const Color& colorKey);
    bool LoadRawTextureData(const std::string& filename, TextureFormat format, std::vector<unsigned char>& native);
    bool LoadCustomTexture(unsigned int width, unsigned int height, TextureFormat format, unsigned char* data);
  };
}  // namespace Theodore

#endif /* Texture2D_h */
