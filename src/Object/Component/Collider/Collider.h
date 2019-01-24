/**
  @brief collider component base class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Collider_h
#define Collider_h

#include "../../../Math/Color.h"
#include "../Component.h"
#include "../../../Graphics/Enumeration.h"
#include "../sub/Physics.h"
#include "../../../Geometry/Ray.h"
#include <string>

namespace Theodore {
  class Collider : public Component {
  public:
    Collider(const std::string& name);
    virtual ~Collider();

    ColliderType GetType() const;
    virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) = 0;

  private:
    virtual void CalculateBoundingVolumes() = 0;

  protected:
    ColliderType mType;
  };
}

#endif /* Collider_h */