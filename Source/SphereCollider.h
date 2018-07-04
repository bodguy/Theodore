#ifndef SphereCollider_h
#define SphereCollider_h

#include "Collider.h"
#include "Vector3d.h"

namespace Quark {
	class SphereCollider : public Collider {
	public:
		SphereCollider(const Vector3d& center, float radius);
		virtual ~SphereCollider();

		virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) override;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Vector3d mCenter;
		float mRadius;
	};
}

#endif /* SphereCollider_h */