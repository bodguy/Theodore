// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef SkinnedMeshRenderer_h
#define SkinnedMeshRenderer_h

#include "Renderer.h"

namespace Theodore {
  class SkinnedMeshRenderer : public Renderer {
  public:
    SkinnedMeshRenderer();
    virtual ~SkinnedMeshRenderer() override;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;
  };
}  // namespace Theodore

#endif /* SkinnedMeshRenderer_h */