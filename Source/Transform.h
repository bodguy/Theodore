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
		void Scale(const Vector3d& axis, Enumeration::Space relativeTo = Enumeration::Self);

		// get world Position, Scale, Rotation
		Vector3d GetPosition() const;
		Vector3d GetScale() const;
		Quaternion GetRotation() const;

		Vector3d GetLocalPosition() const;
		Vector3d GetLocalScale() const;
		Quaternion GetLocalRotation() const;

		// get world Forward, Up, Right
		Vector3d GetForward() const;
		Vector3d GetUp() const;
		Vector3d GetRight() const;

		Vector3d GetEulerAngles() const;
		Vector3d GetLocalEulerAngles() const;

		Matrix4x4 GetLocalToWorldMatrix() const;
		Matrix4x4 GetWorldToLocalMatrix() const;

		void SetPosition(const Vector3d& position);
		void SetLossyScale(const Vector3d& scale);
		void SetEulerAngles(const Vector3d& euler);
		void SetRotation(const Quaternion& quat);

		void SetLocalPosition(const Vector3d& position);
		void SetLocalScale(const Vector3d& scale);
		void SetLocalEulerAngles(const Vector3d& euler);
		void SetLocalRotation(const Quaternion& quat);

		void LookAt(const Transform& target, const Vector3d& worldUp = Vector3d::up);
		Vector3d TransformDirection(const Vector3d& direction);

		GameObject* GetParent() const;
		void SetParent(GameObject* parent);
		
	private:
		Matrix4x4 GetWorldMatrix() const;
		Matrix4x4 GetLocalMatrix() const;
		Matrix4x4 GetParentLocalMatrix() const;

		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Vector3d mPosition;
		Quaternion mRotation;
		Vector3d mLossyScale;

		Vector3d mLocalPosition;
		Quaternion mLocalRotation;
		Vector3d mLocalScale;
		mutable Vector3d mLocalEulerAngles;

		mutable Vector3d mForward;
		mutable Vector3d mUp;
		mutable Vector3d mRight;

		mutable Vector3d mEulerAngles;
		mutable Matrix4x4 mLocalToWorldMatrix;
		mutable Matrix4x4 mWorldToLocalMatrix;
	};
}

#endif /* Transform_h */