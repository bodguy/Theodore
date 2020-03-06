// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Transform.h"

#include "Math/Mathf.h"
#include "Core/GameObject.h"

namespace Theodore {
  Transform::Transform() : Component("Transform"), rotation(), localToWorldMatrix(), worldToLocalMatrix(), localRotation() {
    position = Vector3d::zero;
    lossyScale = Vector3d::one;

    localPosition = Vector3d::zero;
    localScale = Vector3d::one;
    localEulerAngles = Vector3d::zero;

    up = rotation * Vector3d::up;
    right = rotation * Vector3d::right;
    forward = rotation * Vector3d::forward;

    eulerAngles = Vector3d::zero;
  }

  Transform::~Transform() {}

  void Transform::Translate(const Vector3d& translation, Space relativeTo) {
    if (relativeTo == Space::Self) {
      localPosition += translation;
    } else if (relativeTo == Space::World) {
      position += translation;
    }
  }

  void Transform::Rotate(const Vector3d& axis, float angle, Space relativeTo) {
    if (relativeTo == Space::Self) {
      localRotation = Quaternion::AngleAxis(Mathf::Radians(angle), Vector3d(axis).Normalize()) * localRotation;
      localRotation.Normalize();
    } else if (relativeTo == Space::World) {
      rotation = Quaternion::AngleAxis(Mathf::Radians(angle), Vector3d(axis).Normalize()) * rotation;
      rotation.Normalize();
    }
  }

  void Transform::Rotate(const Vector3d& eulerAngles, Space relativeTo) {
    if (relativeTo == Space::Self) {
      localRotation = Quaternion::FromEuler(Vector3d(Mathf::Radians(eulerAngles.x), Mathf::Radians(eulerAngles.y), Mathf::Radians(eulerAngles.z))) * localRotation;
      localRotation.Normalize();
    } else if (relativeTo == Space::World) {
      rotation = Quaternion::FromEuler(Vector3d(Mathf::Radians(eulerAngles.x), Mathf::Radians(eulerAngles.y), Mathf::Radians(eulerAngles.z))) * rotation;
      rotation.Normalize();
    }
  }

  void Transform::RotateAround(const Vector3d& point, const Vector3d& axis, float angle) {
    // TODO
  }

  void Transform::Scale(const Vector3d& axis, Space relativeTo) {
    if (relativeTo == Space::Self) {
      localScale += axis;
    } else if (relativeTo == Space::World) {
      lossyScale += axis;
    }
  }

  Vector3d Transform::GetPosition() const { return localPosition + position; }

  Vector3d Transform::GetScale() const { return localScale + lossyScale; }

  Quaternion Transform::GetRotation() const { return localRotation * rotation; }

  Vector3d Transform::GetLocalPosition() const { return localPosition; }

  Vector3d Transform::GetLocalScale() const { return localScale; }

  Quaternion Transform::GetLocalRotation() const { return localRotation; }

  Vector3d Transform::GetForward() const {
    forward = GetRotation() * Vector3d::forward;
    return forward.Normalize();
  }

  Vector3d Transform::GetUp() const {
    up = GetRotation() * Vector3d::up;
    return up.Normalize();
  }

  Vector3d Transform::GetRight() const {
    right = Vector3d::CrossProduct(GetForward(), GetUp());
    return right.Normalize();
  }

  Vector3d Transform::GetEulerAngles() const {
    eulerAngles = GetLocalEulerAngles() * Quaternion::ToEuler(rotation);
    return eulerAngles;
  }

  Vector3d Transform::GetLocalEulerAngles() const {
    localEulerAngles = Quaternion::ToEuler(localRotation);
    return localEulerAngles;
  }

  Matrix4x4 Transform::GetLocalToWorldMatrix() const {
    localToWorldMatrix = GetLocalMatrix() * GetWorldMatrix() * GetParentLocalMatrix();
    return localToWorldMatrix;
  }

  Matrix4x4 Transform::GetWorldToLocalMatrix() const {
    worldToLocalMatrix = Matrix4x4::Inverse(GetLocalToWorldMatrix());
    return worldToLocalMatrix;
  }

  void Transform::SetPosition(const Vector3d& position) { this->position = position; }

  void Transform::SetLossyScale(const Vector3d& scale) { lossyScale = scale; }

  void Transform::SetEulerAngles(const Vector3d& euler) {
    rotation = Quaternion::FromEuler(Vector3d(Mathf::Radians(euler.x), Mathf::Radians(euler.y), Mathf::Radians(euler.z)));
    rotation.Normalize();
  }

  void Transform::SetRotation(const Quaternion& quat) { rotation = quat; }

  void Transform::SetLocalPosition(const Vector3d& position) { localPosition = position; }

  void Transform::SetLocalScale(const Vector3d& scale) { localScale = scale; }

  void Transform::SetLocalEulerAngles(const Vector3d& euler) {
    localRotation = Quaternion::FromEuler(Vector3d(Mathf::Radians(euler.x), Mathf::Radians(euler.y), Mathf::Radians(euler.z)));
    localRotation.Normalize();
  }

  void Transform::SetLocalRotation(const Quaternion& quat) { localRotation = quat; }

  void Transform::LookAt(const Transform& target, const Vector3d& worldUp) {
    forward = (target.position - position).Normalize();
    // Now we get the perpendicular projection of the viewForward vector onto the world up vector
    // Uperp = U - ( U.V / V.V ) * V
    up = (worldUp - (Vector3d::Project(forward, worldUp))).Normalize();
    // Alternatively for getting viewUp you could just use:
    // viewUp = thisTransform.TransformDirection(thisTransform.up);
    // viewUp.Normalize();
    right = Vector3d::CrossProduct(up, forward);
  }

  Matrix4x4 Transform::GetWorldMatrix() const { return Matrix4x4::Scale(lossyScale) * Quaternion::ToRotationMatrix(rotation) * Matrix4x4::Translate(position); }

  Matrix4x4 Transform::GetLocalMatrix() const { return Matrix4x4::Scale(localScale) * Quaternion::ToRotationMatrix(localRotation) * Matrix4x4::Translate(localPosition); }

  Matrix4x4 Transform::GetParentLocalMatrix() const {
    if (!gameObject || !gameObject->parent) {
      return Matrix4x4::Identity();
    }

    return gameObject->parent->GetTransform()->GetLocalMatrix() * gameObject->parent->GetTransform()->GetParentLocalMatrix();
  }

  // private functions, for consistency with other components.

  void Transform::Update(float deltaTime) {}

  void Transform::Render() { return; }

  bool Transform::CompareEquality(const Object& rhs) const { return false; }

  bool Transform::Destroy() { return false; }

  GameObject* Transform::GetParent() const { return this->gameObject->parent; }

  void Transform::SetParent(GameObject* parent) {
    this->gameObject->parent = parent;
    this->gameObject->parent->children.push_back(this->gameObject);
  }

  Vector3d Transform::InverseTransformDirection(const Vector3d& direction) const {
    Matrix4x4 model = GetWorldToLocalMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * direction;
  }

  Vector3d Transform::InverseTransformPoint(const Vector3d& position) const {
    Matrix4x4 model = GetWorldToLocalMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * (position * Matrix4x4::DecomposeScale(model)) + Matrix4x4::DecomposeTranslation(model);
  }

  Vector3d Transform::InverseTransformVector(const Vector3d& vector) const {
    Matrix4x4 model = GetWorldToLocalMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * (vector * Matrix4x4::DecomposeScale(model));
  }

  Vector3d Transform::TransformDirection(const Vector3d& direction) const {
    Matrix4x4 model = GetLocalToWorldMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * direction;
  }

  Vector3d Transform::TransformPoint(const Vector3d& position) const {
    Matrix4x4 model = GetLocalToWorldMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * (position * Matrix4x4::DecomposeScale(model)) + Matrix4x4::DecomposeTranslation(model);
  }

  Vector3d Transform::TransformVector(const Vector3d& vector) const {
    Matrix4x4 model = GetLocalToWorldMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * (vector * Matrix4x4::DecomposeScale(model));
  }
}  // namespace Theodore