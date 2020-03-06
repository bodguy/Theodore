// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Plane.h"
#include "Math/Mathf.h"

namespace Theodore {
  Plane::Plane(Vector3d inNormal, const Vector3d& inPoint) {
    // ax + by + cz + d = 0
    // d = -(ax1 + by1 + cz1)
    // d = -((x1, y1, z1) * (a,b,c))
    normal = inNormal.Normalize();
		distance = -Vector3d::DotProduct(inPoint, normal);
  }

  Plane::Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c) {
		normal = Vector3d::CrossProduct(b - a, c - a).Normalize();
		distance = -Vector3d::DotProduct(a, normal);
  }

  Plane::~Plane() {}

  bool Plane::Raycast(const Ray& r, float* enter) {
    float numer = Vector3d::DotProduct(r.origin, normal) + distance;
    float denom = Vector3d::DotProduct(r.direction, normal);

    if (denom < Mathf::flt_epsilon) {  // normal is orthogonal to vector, cant intersect
      *enter = -1.f;
      return false;
    }
    *enter = -(numer / denom);
    return true;
  }
}  // namespace Theodore