/**
  @brief mathmatical plane class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Plane_h
#define Plane_h

#include "Ray.h"
#include "Vector3d.h"

namespace Theodore {
  class Plane {
  public:
    Plane(Vector3d inNormal, const Vector3d& inPoint);
    Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
    ~Plane();

    bool Raycast(const Ray& r, float* enter);

  private:
    Vector3d n;
    float d;
  };
}

#endif // Plane_h