// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef TextureCube_h
#define TextureCube_h

#include <string>

#include "Graphics/Enumeration.h"
#include "Texture.h"

namespace Theodore {
  class CubemapRenderer;
  class TextureCube : public Texture {
  public:
    TextureCube();
    ~TextureCube();

    bool LoadCubemapTexture(const CubemapRenderer* cubemap, const std::string& filename, TextureFormat format, CubemapFace face);
    CubemapFace GetFace() const;

  private:
    CubemapFace cubemapFace;
  };
}  // namespace Theodore

#endif /* TextureCube_h */
