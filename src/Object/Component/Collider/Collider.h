/**
  @brief collider component base class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Collider_h
#define Collider_h

#include <string>
#include "../../../Geometry/Ray.h"
#include "../../../Graphics/Enumeration.h"
#include "../../../Math/Color.h"
#include "../Component.h"
#include "../sub/Physics.h"

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
}  // namespace Theodore

#endif /* Collider_h */