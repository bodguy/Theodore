#include "Ray.h"
#include "Math.h"

namespace Quark {
	Ray::Ray() : Origin(), Direction() {

	}

	Ray::Ray(const Vector3d& origin, const Vector3d& direction) : Origin(origin), Direction(direction) {

	}

	Ray::Ray(const Ray& other) {
		Origin = other.Origin;
		Direction = other.Direction;
	}

	Vector3d Ray::GetPoint(float distance) {
		return Origin + Direction * distance;
	}

	Ray& Ray::operator *(Matrix4x4& matrix) {
		Vector3d tmp;
		tmp = Origin + Direction;
		tmp = matrix * tmp;
		Origin = matrix * Origin;
		Direction = tmp - Origin;

		return *this;
	}

	bool Ray::operator ==(const Ray& other) {
		return ((Origin == other.Origin) && (Direction == other.Direction));
	}

	bool Ray::operator !=(const Ray& other) {
		return !(*this == other);
	}
}