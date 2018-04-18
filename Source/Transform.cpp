#include "Transform.h"
#include "Math.h"

namespace Quark {
	Transform::Transform() : mRotation(), mLocalToWorldMatrix(), mWorldToLocalMatrix() {
		mPosition = Vector3d::zero;
		mScale = Vector3d::one;

		mUp = mRotation * Vector3d::up;
		mRight = mRotation * Vector3d::right;
		mForward = mRotation * Vector3d::forward;

		mEulerAngles = Vector3d::zero;
	}

	void Transform::Translate(const Vector3d& translation, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			mPosition += translation;
		} else if(relativeTo == Enumeration::World) {

		}
	}

	void Transform::Rotate(const Vector3d& axis, float angle, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			mRotation = Quaternion::AngleAxis(angle, axis);
		} else if(relativeTo == Enumeration::World) {

		}
	}

	void Transform::Rotate(const Vector3d& eulerAngles, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			Quaternion rhs = Quaternion::FromEuler(eulerAngles);
			mRotation *= Quaternion::Inverse(mRotation) * rhs * mRotation;
		} else if(relativeTo == Enumeration::World) {

		}
	}

	Vector3d Transform::GetPosition() const {
		return mPosition;
	}

	Vector3d Transform::GetScale() const {
		return mScale;
	}

	Vector3d Transform::GetRotation() const {
		Vector3d euler = Quaternion::ToEuler(mRotation);
		return euler;
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
		mRight = mRotation * Vector3d::right;
		return mRight;
	}

	Vector3d Transform::GetEulerAngles() const {
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
		mRotation = Quaternion::FromEuler(euler);
	}
}