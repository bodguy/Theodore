/**
  @brief mahmatical ray class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Ray_h
#define Ray_h

#include "../Math/Matrix4x4.h"
#include "../Math/Vector3d.h"

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
    Vector3d invDirection;
  };
} // namespace Theodore

#endif /* Ray_h */