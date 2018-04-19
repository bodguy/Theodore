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
			// this is problem
			Quaternion quat = mRotation;
			Quaternion quatInv = Quaternion::Inverse(mRotation);
			quat.Normalize();
			quatInv.Normalize();

			mRotation *= quat * Quaternion::AngleAxis(Math::Radians(angle), axis) * quatInv;
			mRotation.Normalize();
		} else if(relativeTo == Enumeration::World) {

		}
	}

	void Transform::Rotate(const Vector3d& eulerAngles, Enumeration::Space relativeTo) {
		if(relativeTo == Enumeration::Self) {
			// this is problem
			Quaternion rhs = Quaternion::FromEuler(Vector3d(Math::Radians(eulerAngles.x), Math::Radians(eulerAngles.y), Math::Radians(eulerAngles.z)));
			mRotation *= rhs;
			mRotation.Normalize();
		} else if(relativeTo == Enumeration::World) {

		}
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
		mRight = mRotation * Vector3d::right;
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
}