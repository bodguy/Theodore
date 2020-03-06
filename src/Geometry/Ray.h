// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Ray_h
#define Ray_h

#include "Math/Matrix4x4.h"
#include "Math/Vector3d.h"

namespace Theodore {
  class Ray {
  public:
    Ray();
    Ray(const Vector3d& origin, const Vector3d& direction);
    Ray(const Ray& other);

    Vector3d GetPoint(float distance);
    bool operator==(const Ray& other);
    bool operator!=(const Ray& other);

    Vector3d origin;
    Vector3d direction;
    Vector3d inverseDirection;
  };
}  // namespace Theodore

#endif /* Ray_h */