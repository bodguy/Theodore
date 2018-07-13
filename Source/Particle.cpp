#include "Particle.h"
#include "Cloth.h"

namespace Theodore {
	Particle::Particle() {

	}

	Particle::Particle(const Vector3d& vec) :mPosition(vec), mOldPosition(vec), mAcceleration(Vector3d(0.f, 0.f, 0.f)), mAccmulatedNormal(Vector3d(0.f, 0.f, 0.f)), mMass(1.f), mMovable(true) {

	}

	Particle::~Particle() {

	}

	Vector3d& Particle::GetPosition() {
		return mPosition;
	}

	Vector3d& Particle::GetNormal() {
		return mAccmulatedNormal;
	}

	void Particle::ResetAcceleration() {
		mAcceleration = Vector3d(0.f, 0.f, 0.f);
	}

	void Particle::ResetNormal() {
		mAccmulatedNormal = Vector3d(0.f, 0.f, 0.f);
	}

	void Particle::OffsetPos(const Vector3d& v) {
		if (mMovable) {
			mPosition += v;
		}
	}

	void Particle::SetMovable(bool b) {
		mMovable = b;
	}

	void Particle::AddToNormal(Vector3d normal) {
		mAccmulatedNormal += normal.Normalize();
	}

	void Particle::AddForce(const Vector3d& f) {
		mAcceleration += f / mMass;
	}

	void Particle::Update(float dt) {
		if (mMovable) {
			Vector3d temp = mPosition;
			mPosition = mPosition + (mPosition - mOldPosition) * (1.0f - Cloth::GetDamping()) + mAcceleration * dt * 3.f;
			mOldPosition = temp;
			ResetAcceleration();
		}
	}
}