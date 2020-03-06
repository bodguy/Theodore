// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "RigidBody2D.h"

#include "Component/Transform.h"
#include "Core/GameObject.h"

namespace Theodore {
  RigidBody2D::RigidBody2D() : Component("RigidBody2D"), position(), velocity(), force() {
		mass = 1.f;
		inertia = mass * (3 * 3 + 10 * 10) / 12;
		rotation = 0.f;
		angularVelocity = 0.f;
		torque = 0.f;
		centerOfMass = Vector2d(3.f / 2.f, 10.f / 2.f);
  }

  RigidBody2D::~RigidBody2D() {}

  void RigidBody2D::AddForce(const Vector2d& force) { this->force += force; }

  void RigidBody2D::AddTorque(float torque) { this->torque += torque; }

  void RigidBody2D::Update(float deltaTime) {
		torque += centerOfMass.x * force.y - centerOfMass.y * force.x;
    Vector2d linearAcceleration = force / mass;
		velocity += linearAcceleration * deltaTime;
		position += velocity * deltaTime;

    float angularAcceleration = torque / inertia;
		angularVelocity += angularAcceleration * deltaTime;
		rotation += angularAcceleration * deltaTime;

    transform->Translate(Vector3d(position, 0.f));
		transform->Rotate(Vector3d::forward, rotation);

		torque = 0.f;
		force = Vector2d::zero;
  }

  void RigidBody2D::Render() {}

  bool RigidBody2D::CompareEquality(const Object& rhs) const { return false; }

  bool RigidBody2D::Destroy() { return false; }
}  // namespace Theodore