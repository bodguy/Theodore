#include "SphereCollider.h"

namespace Quark {
	SphereCollider::SphereCollider(const Vector3d& center, float radius) : Collider("SphereCollider"), mCenter(center), mRadius(radius) {
		mType = ColliderType::Sphere;
	}

	SphereCollider::~SphereCollider() {
	
	}

	bool SphereCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
		Vector3d L = mCenter - ray.origin;
		float tca = Vector3d::DotProduct(L, ray.direction);
		float d2 = Vector3d::DotProduct(L, L) - tca * tca;
		if (d2 > mRadius) 
			return false;

		float thc = std::sqrt(mRadius - d2);
		float t0 = tca - thc;
		float t1 = tca + thc;

		if (t0 > t1) 
			std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return false; // both t0 and t1 are negative 
		}

		return true;
	}

	void SphereCollider::Update(double deltaTime) {

	}

	void SphereCollider::Render() {

	}

	bool SphereCollider::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool SphereCollider::Destroy() {
		return false;
	}
}