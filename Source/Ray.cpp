#include "Ray.h"
#include "Math.h"

namespace Theodore {
	Ray::Ray() : origin(), direction(), invDirection() {

	}

	Ray::Ray(const Vector3d& origin, const Vector3d& direction) : origin(origin), direction(direction) {
		invDirection = Vector3d::Inverse(direction);
	}

	Ray::Ray(const Ray& other) {
		origin = other.origin;
		direction = other.direction;
		invDirection = other.invDirection;
	}

	Vector3d Ray::GetPoint(float distance) {
		return origin + direction * distance;
	}

	bool Ray::operator ==(const Ray& other) {
		return ((origin == other.origin) && (direction == other.direction) && invDirection == other.invDirection);
	}

	bool Ray::operator !=(const Ray& other) {
		return !(*this == other);
	}
}