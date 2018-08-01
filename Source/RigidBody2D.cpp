#include "RigidBody2D.h"
#include "GameObject.h"
#include "Transform.h"

namespace Theodore {
	RigidBody2D::RigidBody2D() : Component("RigidBody2D"), mPosition(), mVelocity(), mForce() {
		mTransform = mGameObject->GetComponent<Transform>();
		mMass = 1.f;
		mInertia = mMass * (3 * 3 + 10 * 10) / 12;
		mRotation = 0.f;
		mAngularVelocity = 0.f;
		mTorque = 0.f;
		mCenterOfMass = Vector2d(3.f/2.f, 10.f/2.f);
	}

	RigidBody2D::~RigidBody2D() {

	}

	void RigidBody2D::AddForce(const Vector2d& force) {
		mForce += force;
	}

	void RigidBody2D::AddTorque(float torque) {
		mTorque += torque;
	}

	void RigidBody2D::Update(float deltaTime) {
		mTorque += mCenterOfMass.x * mForce.y - mCenterOfMass.y * mForce.x;
		Vector2d linearAcceleration = mForce / mMass;
		mVelocity += linearAcceleration * deltaTime;
		mPosition += mVelocity * deltaTime;

		float angularAcceleration = mTorque / mInertia;
		mAngularVelocity += angularAcceleration * deltaTime;
		mRotation += angularAcceleration * deltaTime;

		mTransform->Translate(Vector3d(mPosition, 0.f));
		mTransform->Rotate(Vector3d::forward, mRotation);

		mTorque = 0.f;
		mForce = Vector2d::zero;
	}

	void RigidBody2D::Render() {

	}

	bool RigidBody2D::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool RigidBody2D::Destroy() {
		return false;
	}
}