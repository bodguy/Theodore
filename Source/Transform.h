#ifndef Transform_h
#define Transform_h

#include "Vector3d.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Enumeration.h"
#include "Component.h"

namespace Quark {
	class Transform : public Component {
	public:
		Transform();
		virtual ~Transform();

		void Translate(const Vector3d& translation, Enumeration::Space relativeTo = Enumeration::Self);
		void Rotate(const Vector3d& axis, float angle, Enumeration::Space relativeTo = Enumeration::Self);
		void Rotate(const Vector3d& eulerAngles, Enumeration::Space relativeTo = Enumeration::Self);

		Vector3d GetPosition() const;
		Vector3d GetScale() const;
		Quaternion GetRotation() const;

		Vector3d GetForward() const;
		Vector3d GetUp() const;
		Vector3d GetRight() const;
		Vector3d GetEulerAngles() const;

		Matrix4x4 GetLocalToWorldMatrix() const;
		Matrix4x4 GetWorldToLocalMatrix() const;

		void SetPosition(const Vector3d& position);
		void SetScale(const Vector3d& scale);
		void SetEulerAngles(const Vector3d& euler);
		void SetRotation(const Quaternion& quat);
		
	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Vector3d mPosition;
		Quaternion mRotation;
		Vector3d mScale;

		mutable Vector3d mForward;
		mutable Vector3d mUp;
		mutable Vector3d mRight;

		mutable Vector3d mEulerAngles;
		mutable Matrix4x4 mLocalToWorldMatrix;
		mutable Matrix4x4 mWorldToLocalMatrix;
	};
}

#endif /* Transform_h */