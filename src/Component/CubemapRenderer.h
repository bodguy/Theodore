// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef CubemapRenderer_h
#define CubemapRenderer_h

#include "MeshRenderer.h"

namespace Theodore {
  class CubemapRenderer : public MeshRenderer {
  public:
    CubemapRenderer();
    virtual ~CubemapRenderer() override;

    unsigned int GetTextureID() const;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

    unsigned int textureId;
  };
}  // namespace Theodore

#endif /* CubemapRenderer_h */