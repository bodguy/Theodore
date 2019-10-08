// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Collider_h
#define Collider_h

#include <string>
#include "Geometry/Ray.h"
#include "Graphics/Enumeration.h"
#include "Math/Color.h"
#include "Object/Component/Component.h"
#include "Object/Component/sub/Physics.h"

namespace Theodore {
  class Collider : public Component {
  public:
    explicit Collider(const std::string& name);
    virtual ~Collider() override;

    ColliderType GetType() const;
    virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) = 0;

  private:
    virtual void CalculateBoundingVolumes() = 0;

  protected:
    ColliderType mType;
  };
}  // namespace Theodore

#endif /* Collider_h */