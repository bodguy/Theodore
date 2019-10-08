// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Physics.h"

namespace Theodore {
  Physics::Physics() {}

  Physics::~Physics() {}

  bool Physics::Raycast(const Vector3d& origin, const Vector3d& directoin, float maxDistance) { return false; }

  bool Physics::Raycast(const Vector3d& origin, const Vector3d& directoin, RaycastHit* hitInfo, float maxDistance) { return false; }

  bool Physics::Raycast(const Ray& ray, float maxDistance) { return false; }

  bool Physics::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance) { return false; }
}  // namespace Theodore