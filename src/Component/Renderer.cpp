// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Renderer.h"

#include "Graphics/VertexBuffer.h"
#include "Helper/Utility.h"

namespace Theodore {
  Renderer::Renderer(const std::string& name) : Component(name), pipeline(nullptr), primitive(Primitive::Triangles), bounds(Vector3d::zero, Vector3d::one), isVisibleGizmos(false) {
    vertexArray = new VertexArray();
    vertexBuffers.clear();
    indexBuffers.clear();
		normalVisualizePipeline = Shader::Find("DebugNormal");
  }

  Renderer::~Renderer() {
    SafeContDealloc(vertexBuffers);
    SafeContDealloc(indexBuffers);
    SafeDealloc(vertexArray);
    // DO NOT DEALLOCATE mProgram.
  }

  void Renderer::SetPrimitive(const Primitive primitive) { primitive = primitive; }

  bool Renderer::GetVisibleGizmos() const { return isVisibleGizmos; }

  void Renderer::SetVisibleGizmos(bool visible) { isVisibleGizmos = visible; }

  Bounds* Renderer::GetBounds() { return &bounds; }
}  // namespace Theodore