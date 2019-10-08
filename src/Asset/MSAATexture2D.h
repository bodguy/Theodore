// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef MSAATexture2D_h
#define MSAATexture2D_h

#include "Graphics/Enumeration.h"
#include "Texture.h"

namespace Theodore {
  class MSAATexture2D : public Texture {
  public:
    MSAATexture2D();
    virtual ~MSAATexture2D() override;

    bool LoadMultiSampleTexture(unsigned int width, unsigned int height, TextureFormat format, unsigned int sample);
  };
}  // namespace Theodore

#endif /* MSAATexture2D_h */