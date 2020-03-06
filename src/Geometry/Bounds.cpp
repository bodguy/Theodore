// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Bounds.h"

#include <cmath>

namespace Theodore {
  Bounds::Bounds(const Vector3d& center, const Vector3d& size) : center(center), size(size) {
    extents = size / 2.f;
    min = center - extents;
    max = center + extents;
  }

  Bounds::~Bounds(void) {}

  Vector3d Bounds::GetMin() const { return min; }

  Vector3d Bounds::GetMax() const { return max; }

  Vector3d Bounds::GetCenter() const { return center; }

  Vector3d Bounds::GetExtents() const { return extents; }

  Vector3d Bounds::GetSize() const { return size; }

  bool Bounds::IntersectRay(const Ray& ray) {
    float t1 = (min[0] - ray.origin[0]) * ray.inverseDirection[0];
    float t2 = (max[0] - ray.origin[0]) * ray.inverseDirection[0];

    float tmin = std::fminf(t1, t2);
    float tmax = std::fmaxf(t1, t2);

    for (unsigned int i = 1; i < 3; i++) {
      t1 = (min[i] - ray.origin[i]) * ray.inverseDirection[i];
      t2 = (max[i] - ray.origin[i]) * ray.inverseDirection[i];

      tmin = std::fmaxf(tmin, std::fminf(t1, t2));
      tmax = std::fminf(tmax, std::fmaxf(t1, t2));
    }

    return tmax > std::fmaxf(tmin, 0.f);
  }

  bool Bounds::Intersect(const Bounds& bounds) {
    if (max.x < bounds.min.x || min.x > bounds.max.x) return false;
    if (max.y < bounds.min.y || min.y > bounds.max.y) return false;
    if (max.z < bounds.min.z || min.z > bounds.max.z) return false;

    return true;
  }

  bool Bounds::Intersect(const Plane& bounds) { return false; }

  bool Bounds::Contains(const Vector3d& point) { return min.x <= point.x && point.x <= max.x && min.y <= point.y && point.y <= max.y && min.z <= point.z && point.z <= max.z; }

  bool Bounds::Contains(const Bounds& bounds) {
    return min.x <= bounds.min.x && min.y <= bounds.min.y && min.z <= bounds.min.z && bounds.max.x <= max.x && bounds.max.y <= max.y && bounds.max.z <= max.z;
  }

  void Bounds::Encapsulate(const Vector3d& point) {
    if (Contains(point)) {
      return;
    }

    // TODO
  }

  void Bounds::Encapsulate(const Bounds& bounds) {
    if (Contains(bounds)) {
      return;
    }

    // TODO
  }

  void Bounds::ClosestPoint(const Vector3d& point) {}

  float Bounds::SqrDistance(const Vector3d& point) {
    if (Contains(point)) {
      return 0.f;
    } else {
      Vector3d maxDist(0.f, 0.f, 0.f);

      if (point.x < min.x)
        maxDist.x = min.x - point.x;
      else if (point.x > max.x)
        maxDist.x = point.x - max.x;

      if (point.y < min.y)
        maxDist.y = min.y - point.y;
      else if (point.y > max.y)
        maxDist.y = point.y - max.y;

      if (point.z < min.z)
        maxDist.z = min.z - point.z;
      else if (point.z > max.z)
        maxDist.z = point.z - max.z;

      return maxDist.SquaredLength();
    }
  }

  float Bounds::Distance(const Vector3d& point) { return std::sqrt(SqrDistance(point)); }

  void Bounds::Expand(float amount) { extents += amount; }

  void Bounds::SetMinMax(const Vector3d& min, const Vector3d& max) {
    min = min;
    max = max;
    center = (max + min) * 0.5f;
    extents = center - min;
    size = extents * 2.f;
  }
}  // namespace Theodore