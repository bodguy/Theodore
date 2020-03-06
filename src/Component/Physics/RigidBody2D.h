// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef RigidBody2D_h
#define RigidBody2D_h

#include "Math/Vector2d.h"
#include "Component/Component.h"

namespace Theodore {
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
    Vector2d position;
    Vector2d velocity;
    Vector2d force;
    Vector2d centerOfMass;

    float mass;
    float inertia;
    float rotation;
    float angularVelocity;
    float torque;
  };
} // namespace Theodore

#endif /* RigidBody2D_h */
