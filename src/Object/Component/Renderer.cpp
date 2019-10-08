// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Renderer.h"
#include "Graphics/VertexBuffer.h"
#include "Helper/Utility.h"

namespace Theodore {
  Renderer::Renderer(const std::string& name) : Component(name), mProgram(nullptr), mPrimitive(Primitive::Triangles), mBounds(Vector3d::zero, Vector3d::one), mIsVisibleGizmos(false) {
    mVao = new VertexArray();
    mVbos.clear();
    mEbos.clear();
    mNormalVisualizeProgram = Shader::Find("DebugNormal");
  }

  Renderer::~Renderer() {
    SafeContDealloc(mVbos);
    SafeContDealloc(mEbos);
    SafeDealloc(mVao);
    // DO NOT DEALLOCATE mProgram.
  }

  void Renderer::SetPrimitive(const Primitive primitive) { mPrimitive = primitive; }

  bool Renderer::GetVisibleGizmos() const { return mIsVisibleGizmos; }

  void Renderer::SetVisibleGizmos(bool visible) { mIsVisibleGizmos = visible; }

  Bounds* Renderer::GetBounds() { return &mBounds; }
}  // namespace Theodore