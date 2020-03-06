// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Renderer_h
#define Renderer_h

#include <vector>

#include "Component.h"
#include "Geometry/Bounds.h"
#include "Graphics/Enumeration.h"

namespace Theodore {
  class GPUBuffer;
  class VertexArray;
  class Pipeline;
  class Renderer : public Component {
  public:
    Renderer(const std::string& name);
    virtual ~Renderer() override;

    void SetPrimitive(const Primitive primitive);
    bool GetVisibleGizmos() const;
    void SetVisibleGizmos(bool visible);
    Bounds* GetBounds();

  protected:
    std::vector<GPUBuffer*> vertexBuffers;
    std::vector<GPUBuffer*> indexBuffers;
    VertexArray* vertexArray;
    Pipeline* pipeline;
    Pipeline* normalVisualizePipeline;
    Primitive primitive;
    // world space bounds
    Bounds bounds;
    bool isVisibleGizmos;
  };
}  // namespace Theodore

#endif /* Renderer_h */