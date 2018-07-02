#include "Ray.h"
#include "Math.h"

namespace Quark {
	Ray::Ray() : origin(), direction() {

	}

	Ray::Ray(const Vector3d& origin, const Vector3d& direction) : origin(origin), direction(direction) {

	}

	Ray::Ray(const Ray& other) {
		origin = other.origin;
		direction = other.direction;
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

		return *this;
	}

	bool Ray::operator ==(const Ray& other) {
		return ((origin == other.origin) && (direction == other.direction));
	}

	bool Ray::operator !=(const Ray& other) {
		return !(*this == other);
	}
}