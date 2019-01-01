#include "Plane.h"
#include "Math.h"

namespace Theodore {
  Plane::Plane(Vector3d inNormal, const Vector3d& inPoint) {
    // ax + by + cz + d = 0
    // d = -(ax1 + by1 + cz1)
    // d = -((x1, y1, z1) * (a,b,c))
    n = inNormal.Normalize();
    d = -Vector3d::DotProduct(inPoint, n);
  }

  Plane::Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c) {
    n = Vector3d::CrossProduct(b - a, c - a).Normalize();
    d = -Vector3d::DotProduct(a, n);
  }

  Plane::~Plane() {}

  bool Plane::Raycast(const Ray& r, float* enter) {
    float numer = Vector3d::DotProduct(r.origin, n) + d;
    float denom = Vector3d::DotProduct(r.direction, n);

    if (denom < Math::flt_epsilon) { // normal is orthogonal to vector, cant intersect
      *enter = -1.f;
      return false;
    }
    *enter = -(numer / denom);
    return true;
  }
}