// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Collider.h"

namespace Theodore {
  Collider::Collider(const std::string& name) : Component(name), colliderType(ColliderType::Undefined) {}

  Collider::~Collider() {}

  ColliderType Collider::GetType() const { return colliderType; }
}  // namespace Theodore