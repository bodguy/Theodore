/**
	@brief transform component for translation, scaling, rotation object in the world space
	@author bodguy
	@date 17.07.17
	@todo RotateAround function implementation
	@bug LookAt function needs to be tested
*/

#ifndef Transform_h
#define Transform_h

#include "Vector3d.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Enumeration.h"
#include "Component.h"

namespace Theodore {
	class Transform : public Component {
	public:
		Transform();
		virtual ~Transform();

		void Translate(const Vector3d& translation, Space relativeTo = Space::Self);
		void Rotate(const Vector3d& axis, float angle, Space relativeTo = Space::Self);
		void Rotate(const Vector3d& eulerAngles, Space relativeTo = Space::Self);
		void RotateAround(const Vector3d& point, const Vector3d& axis, float angle);
		void Scale(const Vector3d& axis, Space relativeTo = Space::Self);

		Vector3d GetPosition() const;
		Vector3d GetScale() const;
		Quaternion GetRotation() const;

		Vector3d GetLocalPosition() const;
		Vector3d GetLocalScale() const;
		Quaternion GetLocalRotation() const;

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

		GameObject* GetParent() const;
		void SetParent(GameObject* parent);

		// Transforms direction from world space to local space. 
		// This operation is not affected by scale or position but only affected by rotate of the transform.
		Vector3d InverseTransformDirection(const Vector3d& direction) const;
		// Transforms position from world space to local space.
		// This operation is affected by rotate, scale and position of the transform.
		Vector3d InverseTransformPoint(const Vector3d& position) const;
		// Transforms vector from world space to local space.
		// This operation is affected by rotate and scale of the transform.
		Vector3d InverseTransformVector(const Vector3d& vector) const;
		// Transforms direction from local space to world space. 
		// This operation is not affected by scale or position but only affected by rotate of the transform.
		Vector3d TransformDirection(const Vector3d& direction) const;
		// Transforms position from local space to world space.
		// This operation is affected by rotate, scale and position of the transform.
		Vector3d TransformPoint(const Vector3d& position) const;
		// Transforms vector from local space to world space.
		// This operation is affected by rotate and scale of the transform.
		Vector3d TransformVector(const Vector3d& vector) const;
		
	private:
		Matrix4x4 GetWorldMatrix() const;
		Matrix4x4 GetLocalMatrix() const;
		Matrix4x4 GetParentLocalMatrix() const;

		virtual void Update(float deltaTime) override;
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