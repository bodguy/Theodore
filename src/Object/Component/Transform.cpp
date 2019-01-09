#include "Transform.h"
#include "GameObject.h"
#include "Math.h"

namespace Theodore {
  Transform::Transform()
      : Component("Transform"), mRotation(), mLocalToWorldMatrix(), mWorldToLocalMatrix(),
        mLocalRotation() {
    mPosition = Vector3d::zero;
    mLossyScale = Vector3d::one;

    mLocalPosition = Vector3d::zero;
    mLocalScale = Vector3d::one;
    mLocalEulerAngles = Vector3d::zero;

    mUp = mRotation * Vector3d::up;
    mRight = mRotation * Vector3d::right;
    mForward = mRotation * Vector3d::forward;

    mEulerAngles = Vector3d::zero;
  }

  Transform::~Transform() {}

  void Transform::Translate(const Vector3d& translation, Space relativeTo) {
    if (relativeTo == Space::Self) {
      mLocalPosition += translation;
    } else if (relativeTo == Space::World) {
      mPosition += translation;
    }
  }

  void Transform::Rotate(const Vector3d& axis, float angle, Space relativeTo) {
    if (relativeTo == Space::Self) {
      mLocalRotation =
          Quaternion::AngleAxis(Math::Radians(angle), Vector3d(axis).Normalize()) * mLocalRotation;
      mLocalRotation.Normalize();
    } else if (relativeTo == Space::World) {
      mRotation =
          Quaternion::AngleAxis(Math::Radians(angle), Vector3d(axis).Normalize()) * mRotation;
      mRotation.Normalize();
    }
  }

  void Transform::Rotate(const Vector3d& eulerAngles, Space relativeTo) {
    if (relativeTo == Space::Self) {
      mLocalRotation =
          Quaternion::FromEuler(Vector3d(Math::Radians(eulerAngles.x), Math::Radians(eulerAngles.y),
                                         Math::Radians(eulerAngles.z))) *
          mLocalRotation;
      mLocalRotation.Normalize();
    } else if (relativeTo == Space::World) {
      mRotation =
          Quaternion::FromEuler(Vector3d(Math::Radians(eulerAngles.x), Math::Radians(eulerAngles.y),
                                         Math::Radians(eulerAngles.z))) *
          mRotation;
      mRotation.Normalize();
    }
  }

  void Transform::RotateAround(const Vector3d& point, const Vector3d& axis, float angle) {
    // TODO
  }

  void Transform::Scale(const Vector3d& axis, Space relativeTo) {
    if (relativeTo == Space::Self) {
      mLocalScale += axis;
    } else if (relativeTo == Space::World) {
      mLossyScale += axis;
    }
  }

  Vector3d Transform::GetPosition() const { return mLocalPosition + mPosition; }

  Vector3d Transform::GetScale() const { return mLocalScale + mLossyScale; }

  Quaternion Transform::GetRotation() const { return mLocalRotation * mRotation; }

  Vector3d Transform::GetLocalPosition() const { return mLocalPosition; }

  Vector3d Transform::GetLocalScale() const { return mLocalScale; }

  Quaternion Transform::GetLocalRotation() const { return mLocalRotation; }

  Vector3d Transform::GetForward() const {
    mForward = GetRotation() * Vector3d::forward;
    return mForward.Normalize();
  }

  Vector3d Transform::GetUp() const {
    mUp = GetRotation() * Vector3d::up;
    return mUp.Normalize();
  }

  Vector3d Transform::GetRight() const {
    mRight = Vector3d::CrossProduct(GetForward(), GetUp());
    return mRight.Normalize();
  }

  Vector3d Transform::GetEulerAngles() const {
    mEulerAngles = GetLocalEulerAngles() * Quaternion::ToEuler(mRotation);
    return mEulerAngles;
  }

  Vector3d Transform::GetLocalEulerAngles() const {
    mLocalEulerAngles = Quaternion::ToEuler(mLocalRotation);
    return mLocalEulerAngles;
  }

  Matrix4x4 Transform::GetLocalToWorldMatrix() const {
    mLocalToWorldMatrix = GetLocalMatrix() * GetWorldMatrix() * GetParentLocalMatrix();
    return mLocalToWorldMatrix;
  }

  Matrix4x4 Transform::GetWorldToLocalMatrix() const {
    mWorldToLocalMatrix = Matrix4x4::Inverse(GetLocalToWorldMatrix());
    return mWorldToLocalMatrix;
  }

  void Transform::SetPosition(const Vector3d& position) { mPosition = position; }

  void Transform::SetLossyScale(const Vector3d& scale) { mLossyScale = scale; }

  void Transform::SetEulerAngles(const Vector3d& euler) {
    mRotation = Quaternion::FromEuler(
        Vector3d(Math::Radians(euler.x), Math::Radians(euler.y), Math::Radians(euler.z)));
    mRotation.Normalize();
  }

  void Transform::SetRotation(const Quaternion& quat) { mRotation = quat; }

  void Transform::SetLocalPosition(const Vector3d& position) { mLocalPosition = position; }

  void Transform::SetLocalScale(const Vector3d& scale) { mLocalScale = scale; }

  void Transform::SetLocalEulerAngles(const Vector3d& euler) {
    mLocalRotation = Quaternion::FromEuler(
        Vector3d(Math::Radians(euler.x), Math::Radians(euler.y), Math::Radians(euler.z)));
    mLocalRotation.Normalize();
  }

  void Transform::SetLocalRotation(const Quaternion& quat) { mLocalRotation = quat; }

  void Transform::LookAt(const Transform& target, const Vector3d& worldUp) {
    mForward = (target.mPosition - mPosition).Normalize();
    // Now we get the perpendicular projection of the viewForward vector onto the world up vector
    // Uperp = U - ( U.V / V.V ) * V
    mUp = (worldUp - (Vector3d::Project(mForward, worldUp))).Normalize();
    // Alternatively for getting viewUp you could just use:
    // viewUp = thisTransform.TransformDirection(thisTransform.up);
    // viewUp.Normalize();
    mRight = Vector3d::CrossProduct(mUp, mForward);
  }

  Matrix4x4 Transform::GetWorldMatrix() const {
    return Matrix4x4::Scale(mLossyScale) * Quaternion::ToRotationMatrix(mRotation) *
           Matrix4x4::Translate(mPosition);
  }

  Matrix4x4 Transform::GetLocalMatrix() const {
    return Matrix4x4::Scale(mLocalScale) * Quaternion::ToRotationMatrix(mLocalRotation) *
           Matrix4x4::Translate(mLocalPosition);
  }

  Matrix4x4 Transform::GetParentLocalMatrix() const {
    if (!mGameObject || !mGameObject->mParent) {
      return Matrix4x4::Identity();
    }

    return mGameObject->mParent->GetTransform()->GetLocalMatrix() *
           mGameObject->mParent->GetTransform()->GetParentLocalMatrix();
  }

  // private functions, for consistency with other components.

  void Transform::Update(float deltaTime) {}

  void Transform::Render() { return; }

  bool Transform::CompareEquality(const Object& rhs) const { return false; }

  bool Transform::Destroy() { return false; }

  GameObject* Transform::GetParent() const { return this->mGameObject->mParent; }

  void Transform::SetParent(GameObject* parent) {
    this->mGameObject->mParent = parent;
    this->mGameObject->mParent->mChildren.push_back(this->mGameObject);
  }

  Vector3d Transform::InverseTransformDirection(const Vector3d& direction) const {
    Matrix4x4 model = GetWorldToLocalMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * direction;
  }

  Vector3d Transform::InverseTransformPoint(const Vector3d& position) const {
    Matrix4x4 model = GetWorldToLocalMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) *
               (position * Matrix4x4::DecomposeScale(model)) +
           Matrix4x4::DecomposeTranslation(model);
  }

  Vector3d Transform::InverseTransformVector(const Vector3d& vector) const {
    Matrix4x4 model = GetWorldToLocalMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) *
           (vector * Matrix4x4::DecomposeScale(model));
  }

  Vector3d Transform::TransformDirection(const Vector3d& direction) const {
    Matrix4x4 model = GetLocalToWorldMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) * direction;
  }

  Vector3d Transform::TransformPoint(const Vector3d& position) const {
    Matrix4x4 model = GetLocalToWorldMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) *
               (position * Matrix4x4::DecomposeScale(model)) +
           Matrix4x4::DecomposeTranslation(model);
  }

  Vector3d Transform::TransformVector(const Vector3d& vector) const {
    Matrix4x4 model = GetLocalToWorldMatrix();
    return Quaternion::FromEuler(Matrix4x4::DecomposeRotation(model)) *
           (vector * Matrix4x4::DecomposeScale(model));
  }
}