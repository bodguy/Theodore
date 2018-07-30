#ifndef RigidBody2D_h
#define RigidBody2D_h

#include "Component.h"
#include "Vector2d.h"

namespace Theodore {
	class Transform;
	class RigidBody2D : public Component {
	public:
		RigidBody2D();
		virtual ~RigidBody2D();

		void AddForce(const Vector2d& force);
		void AddTorque(float torque);

	private:
		virtual void Update(double deltaTime) override;
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
}

#endif /* RigidBody2D_h */

