#ifndef SphereCollider_h
#define SphereCollider_h

#include "Collider.h"
#include "Vector3d.h"

namespace Theodore {
	class SphereCollider : public Collider {
	public:
		SphereCollider();
		virtual ~SphereCollider();

		virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) override;
		
		Vector3d GetCenter() const;
		void SetCenter(const Vector3d& center);
		float GetRadius() const;
		void SetRadius(float radius);

	private:
		virtual void CalculateBoundingVolumes() override;

		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		// local space center
		Vector3d mCenter;
		// local space radius
		float mRadius;
		Vector3d mMaxLengthVector;
	};
}

#endif /* SphereCollider_h */