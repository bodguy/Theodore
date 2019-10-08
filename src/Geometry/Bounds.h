// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Bounds_h
#define Bounds_h

#include "Math/Vector3d.h"
#include "Ray.h"

namespace Theodore {
  class Plane;
  class Bounds {
  public:
    Bounds(const Vector3d& center, const Vector3d& size);
    ~Bounds(void);

    Vector3d GetMin() const;
    Vector3d GetMax() const;
    Vector3d GetCenter() const;
    Vector3d GetExtents() const;
    Vector3d GetSize() const;

    bool IntersectRay(const Ray& ray);
    bool Intersect(const Bounds& bounds);
    bool Intersect(const Plane& bounds);
    bool Contains(const Vector3d& point);
    bool Contains(const Bounds& bounds);
    void Encapsulate(const Vector3d& point);
    void Encapsulate(const Bounds& bounds);
    void ClosestPoint(const Vector3d& point);
    float SqrDistance(const Vector3d& point);
    float Distance(const Vector3d& point);
    void Expand(float amount);
    void SetMinMax(const Vector3d& min, const Vector3d& max);

  private:
    // The minimal point of the box. This is always equal to center-extents.
    Vector3d mMin;
    // The maximal point of the box.This is always equal to center + extents.
    Vector3d mMax;
    // The center of the bounding box.
    Vector3d mCenter;
    // The extents of the Bounding Box. This is always half of the size of the Bounds.
    Vector3d mExtents;
    // The total size of the box. This is always twice as large as the extents.
    Vector3d mSize;
  };
}  // namespace Theodore

#endif /* Bounds_h */