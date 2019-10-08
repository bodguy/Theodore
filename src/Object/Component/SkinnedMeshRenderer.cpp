// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "SkinnedMeshRenderer.h"

namespace Theodore {
  SkinnedMeshRenderer::SkinnedMeshRenderer() : Renderer("SkinnedMeshRenderer") {}

  SkinnedMeshRenderer::~SkinnedMeshRenderer() {}

  void SkinnedMeshRenderer::Update(float deltaTime) {}

  void SkinnedMeshRenderer::Render() {}

  bool SkinnedMeshRenderer::CompareEquality(const Object& rhs) const { return false; }

  bool SkinnedMeshRenderer::Destroy() { return false; }
}  // namespace Theodore