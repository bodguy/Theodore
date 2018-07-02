#include "Physics.h"

namespace Quark {
	Physics::Physics() {

	}

	Physics::~Physics() {

	}

	bool Physics::Raycast(const Vector3d& origin, const Vector3d& directoin, float maxDistance) {
		return false;
	}

	bool Physics::Raycast(const Vector3d& origin, const Vector3d& directoin, RaycastHit* hitInfo, float maxDistance) {
		return false;
	}

	bool Physics::Raycast(const Ray& ray, float maxDistance) {
		return false;
	}

	bool Physics::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance) {
		return false;
	}
}