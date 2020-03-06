// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef BoxCollider_h
#define BoxCollider_h

#include "Collider.h"
#include "Math/Vector3d.h"

namespace Theodore {
  class BoxCollider : public Collider {
  public:
    BoxCollider();
    virtual ~BoxCollider() override;

    Vector3d GetCenter() const;
    void SetCenter(const Vector3d& center);
    Vector3d GetSize() const;
    void SetSize(const Vector3d& size);

    virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) override;

  private:
    virtual void CalculateBoundingVolumes() override;

    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  private:
    // local space center
    Vector3d center;
    // local space size
    Vector3d size;
  };
}  // namespace Theodore

#endif /* BoxCollider_h */