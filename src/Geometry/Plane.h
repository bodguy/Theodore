// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Plane_h
#define Plane_h

#include "Math/Vector3d.h"
#include "Ray.h"

namespace Theodore {
  class Plane {
  public:
    Plane(Vector3d inNormal, const Vector3d& inPoint);
    Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
    ~Plane();

    bool Raycast(const Ray& r, float* enter);

  private:
    Vector3d normal;
    float distance;
  };
}  // namespace Theodore

#endif  // Plane_h