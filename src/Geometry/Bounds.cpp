// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Bounds.h"
#include <cmath>

namespace Theodore {
  Bounds::Bounds(const Vector3d& center, const Vector3d& size) : mCenter(center), mSize(size) {
    mExtents = size / 2.f;
    mMin = center - mExtents;
    mMax = center + mExtents;
  }

  Bounds::~Bounds(void) {}

  Vector3d Bounds::GetMin() const { return mMin; }

  Vector3d Bounds::GetMax() const { return mMax; }

  Vector3d Bounds::GetCenter() const { return mCenter; }

  Vector3d Bounds::GetExtents() const { return mExtents; }

  Vector3d Bounds::GetSize() const { return mSize; }

  bool Bounds::IntersectRay(const Ray& ray) {
    float t1 = (mMin[0] - ray.origin[0]) * ray.invDirection[0];
    float t2 = (mMax[0] - ray.origin[0]) * ray.invDirection[0];

    float tmin = std::fminf(t1, t2);
    float tmax = std::fmaxf(t1, t2);

    for (unsigned int i = 1; i < 3; i++) {
      t1 = (mMin[i] - ray.origin[i]) * ray.invDirection[i];
      t2 = (mMax[i] - ray.origin[i]) * ray.invDirection[i];

      tmin = std::fmaxf(tmin, std::fminf(t1, t2));
      tmax = std::fminf(tmax, std::fmaxf(t1, t2));
    }

    return tmax > std::fmaxf(tmin, 0.f);
  }

  bool Bounds::Intersect(const Bounds& bounds) {
    if (mMax.x < bounds.mMin.x || mMin.x > bounds.mMax.x) return false;
    if (mMax.y < bounds.mMin.y || mMin.y > bounds.mMax.y) return false;
    if (mMax.z < bounds.mMin.z || mMin.z > bounds.mMax.z) return false;

    return true;
  }

  bool Bounds::Intersect(const Plane& bounds) { return false; }

  bool Bounds::Contains(const Vector3d& point) { return mMin.x <= point.x && point.x <= mMax.x && mMin.y <= point.y && point.y <= mMax.y && mMin.z <= point.z && point.z <= mMax.z; }

  bool Bounds::Contains(const Bounds& bounds) {
    return mMin.x <= bounds.mMin.x && mMin.y <= bounds.mMin.y && mMin.z <= bounds.mMin.z && bounds.mMax.x <= mMax.x && bounds.mMax.y <= mMax.y && bounds.mMax.z <= mMax.z;
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

      if (point.x < mMin.x)
        maxDist.x = mMin.x - point.x;
      else if (point.x > mMax.x)
        maxDist.x = point.x - mMax.x;

      if (point.y < mMin.y)
        maxDist.y = mMin.y - point.y;
      else if (point.y > mMax.y)
        maxDist.y = point.y - mMax.y;

      if (point.z < mMin.z)
        maxDist.z = mMin.z - point.z;
      else if (point.z > mMax.z)
        maxDist.z = point.z - mMax.z;

      return maxDist.SquaredLength();
    }
  }

  float Bounds::Distance(const Vector3d& point) { return std::sqrt(SqrDistance(point)); }

  void Bounds::Expand(float amount) { mExtents += amount; }

  void Bounds::SetMinMax(const Vector3d& min, const Vector3d& max) {
    mMin = min;
    mMax = max;
    mCenter = (mMax + mMin) * 0.5f;
    mExtents = mCenter - mMin;
    mSize = mExtents * 2.f;
  }
}  // namespace Theodore