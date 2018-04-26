#include "Transform.h"
#include "Math.h"
#include "Utility.h"

namespace Quark {
	Transform::Transform() : Component("Transform"), mRotation(), mLocalToWorldMatrix(), mWorldToLocalMatrix() {
		mPosition = Vector3d::zero;
		mScale = Vector3d::one;

		mUp = mRotation * Vector3d::up;
		mRight = mRotation * Vector3d::right;
		mForward = mRotation * Vector3d::forward;

		mEulerAngles = Vector3d::zero;
	}

	Transform::~Transform() {

	}

	void Transform::Translate(const Vector3d& translation, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			mPosition += translation;
		} else if(relativeTo == Enumeration::World) {

		}
	}

	void Transform::Rotate(const Vector3d& axis, float angle, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			// this is problem
			mRotation = Quaternion::AngleAxis(Math::Radians(angle), Vector3d(axis).Normalize()) * mRotation;
			
		} else if(relativeTo == Enumeration::World) {
			
		}
		mRotation.Normalize();
	}

	void Transform::Rotate(const Vector3d& eulerAngles, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			// this is problem
			Quaternion rhs = Quaternion::FromEuler(Vector3d(Math::Radians(eulerAngles.x), Math::Radians(eulerAngles.y), Math::Radians(eulerAngles.z)));
			mRotation = rhs * mRotation;
			
		} else if(relativeTo == Enumeration::World) {

		}
		mRotation.Normalize();
	}

	Vector3d Transform::GetPosition() const {
		return mPosition;
	}

	Vector3d Transform::GetScale() const {
		return mScale;
	}

	Quaternion Transform::GetRotation() const {
		return mRotation;
	}

	Vector3d Transform::GetForward() const {
		mForward = mRotation * Vector3d::forward;
		return mForward;
	}

	Vector3d Transform::GetUp() const {
		mUp = mRotation * Vector3d::up;
		return mUp;
	}

	Vector3d Transform::GetRight() const {
		mRight = Vector3d::CrossProduct(GetForward(), GetUp());
		return mRight;
	}

	Vector3d Transform::GetEulerAngles() const {
		mEulerAngles = Quaternion::ToEuler(mRotation);
		return mEulerAngles;
	}

	Matrix4x4 Transform::GetLocalToWorldMatrix() const {
		mLocalToWorldMatrix = Matrix4x4::Scale(mScale) * Quaternion::ToRotationMatrix(mRotation) * Matrix4x4::Translate(mPosition);
		return mLocalToWorldMatrix;
	}

	Matrix4x4 Transform::GetWorldToLocalMatrix() const {
		mWorldToLocalMatrix = Matrix4x4(mLocalToWorldMatrix).MakeInverse();
		return mWorldToLocalMatrix;
	}

	void Transform::SetPosition(const Vector3d& position) {
		mPosition = position;
	}

	void Transform::SetScale(const Vector3d& scale) {
		mScale = scale;
	}

	void Transform::SetEulerAngles(const Vector3d& euler) {
		mRotation = Quaternion::FromEuler(Vector3d(Math::Radians(euler.x), Math::Radians(euler.y), Math::Radians(euler.z)));
		mRotation.Normalize();
	}

	void Transform::SetRotation(const Quaternion& quat) {
		mRotation = quat;
	}

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

	Vector3d Transform::TransformDirection(const Vector3d& direction) {
		// TODO
		return Vector3d();
	}

	// private functions, for consistency with other components.

	void Transform::Update(double deltaTime) {
		UnUsed(deltaTime);
	}

	void Transform::Render() {
		return;
	}

	bool Transform::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool Transform::Destroy() {
		return false;
	}
}