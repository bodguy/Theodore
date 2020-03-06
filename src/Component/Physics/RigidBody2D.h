// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef RigidBody2D_h
#define RigidBody2D_h

#include "Math/Vector2d.h"
#include "Component/Component.h"

namespace Theodore {
  class Transform;
  class RigidBody2D : public Component {
  public:
    RigidBody2D();
    virtual ~RigidBody2D() override;

    void AddForce(const Vector2d& force);
    void AddTorque(float torque);

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  private:
    Transform* mTransform;
    Vector2d mPosition;
    Vector2d mVelocity;
    Vector2d mForce;
    Vector2d mCenterOfMass;

    float mMass;
    float mInertia;
    float mRotation;
    float mAngularVelocity;
    float mTorque;
  };
}  // namespace Theodore

#endif /* RigidBody2D_h */
