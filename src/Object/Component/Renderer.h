// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Renderer_h
#define Renderer_h

#include <vector>
#include "Component.h"
#include "Geometry/Bounds.h"
#include "Graphics/Enumeration.h"

namespace Theodore {
  class Buffer;
  class VertexArray;
  class Pipeline;
  class Renderer : public Component {
  public:
    Renderer(const std::string& name);
    virtual ~Renderer();

    void SetPrimitive(const Primitive primitive);
    bool GetVisibleGizmos() const;
    void SetVisibleGizmos(bool visible);
    Bounds* GetBounds();

  protected:
    std::vector<Buffer*> mVbos;
    std::vector<Buffer*> mEbos;
    VertexArray* mVao;
    Pipeline* mProgram;
    Pipeline* mNormalVisualizeProgram;
    Primitive mPrimitive;
    // world space bounds
    Bounds mBounds;
    bool mIsVisibleGizmos;
  };
}  // namespace Theodore

#endif /* Renderer_h */