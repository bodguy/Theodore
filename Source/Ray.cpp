#include "Ray.h"
#include "Math.h"

namespace Quark {
	Ray::Ray() : mOrigin(), mDirection() {

	}

	Ray::Ray(const Vector3d& origin, const Vector3d& direction) : mOrigin(origin), mDirection(direction) {

	}

	Ray::Ray(const Ray& other) {
		mOrigin = other.mOrigin;
		mDirection = other.mDirection;
	}

	Vector3d Ray::GetPoint(float distance) {
		return mOrigin + mDirection * distance;
	}

	Ray& Ray::operator *(Matrix4x4& matrix) {
		Vector3d tmp;
		tmp = mOrigin + mDirection;
		tmp = matrix * tmp;
		mOrigin = matrix * mOrigin;
		mDirection = tmp - mOrigin;

		return *this;
	}

	bool Ray::operator ==(const Ray& other) {
		return ((mOrigin == other.mOrigin) && (mDirection == other.mDirection));
	}

	bool Ray::operator !=(const Ray& other) {
		return !(*this == other);
	}
}