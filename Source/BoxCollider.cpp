#include "BoxCollider.h"

namespace Quark {
	BoxCollider::BoxCollider() : Collider("BoxCollider") {
		mType = ColliderType::Box;
	}

	BoxCollider::~BoxCollider() {
	
	}

	bool BoxCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
		Vector3d min = mCenter - mSize * 0.5f;
		Vector3d max = min - mCenter * 2.f;

		float t1 = (min[0] - ray.origin[0]) * ray.invDirection[0];
		float t2 = (max[0] - ray.origin[0]) * ray.invDirection[0];

		float tmin = std::fminf(t1, t2);
		float tmax = std::fmaxf(t1, t2);

		for (unsigned int i = 1; i < 3; i++) {
			t1 = (min[i] - ray.origin[i]) * ray.invDirection[i];
			t2 = (max[i] - ray.origin[i]) * ray.invDirection[i];

			tmin = std::fmaxf(tmin, std::fminf(t1, t2));
			tmax = std::fminf(tmax, std::fmaxf(t1, t2));
		}

		return tmax > std::fmaxf(tmin, 0.f);
	}

	void BoxCollider::Update(double deltaTime) {

	}

	void BoxCollider::Render() {

	}

	bool BoxCollider::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool BoxCollider::Destroy() {
		return false;
	}
}