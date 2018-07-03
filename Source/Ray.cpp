#include "Ray.h"
#include "Math.h"

namespace Quark {
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

	Ray& Ray::operator *(Matrix4x4& matrix) {
		Vector3d tmp;
		tmp = origin + direction;
		tmp = matrix * tmp;
		origin = matrix * origin;
		direction = tmp - origin;
		invDirection = Vector3d::Inverse(direction);

		return *this;
	}

	bool Ray::operator ==(const Ray& other) {
		return ((origin == other.origin) && (direction == other.direction) && invDirection == other.invDirection);
	}

	bool Ray::operator !=(const Ray& other) {
		return !(*this == other);
	}
}