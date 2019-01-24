/**
  @brief physical calculation
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Physics_h
#define Physics_h

#include "../../../Math/Math.h"
#include "../../../Geometry/Ray.h"
#include "../../../Math/Vector3d.h"

namespace Theodore {
  class RaycastHit {
  public:
    RaycastHit() {}
    ~RaycastHit() {}
  };

  class Physics {
  public:
    Physics();
    ~Physics();

    static bool Raycast(const Vector3d& origin, const Vector3d& directoin,
                        float maxDistance = Math::infinity);
    static bool Raycast(const Vector3d& origin, const Vector3d& directoin, RaycastHit* hitInfo,
                        float maxDistance = Math::infinity);
    static bool Raycast(const Ray& ray, float maxDistance = Math::infinity);
    static bool Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance = Math::infinity);

  private:
  };
}

#endif Physics_h /* Physics_h */