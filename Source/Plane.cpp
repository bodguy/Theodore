#include "Plane.h"
#include "Math.h"

namespace Quark {
	Plane::Plane(Vector3d inNormal, const Vector3d& inPoint) {
		// ax + by + cz + d = 0
		// d = -(ax1 + by1 + cz1)
		// d = -((x1, y1, z1) * (a,b,c))
		Vector3d normal = inNormal.Normalize();
		mNormal.x = normal.x;
		mNormal.y = normal.y;
		mNormal.z = normal.z;
		d = -Vector3d::DotProduct(inPoint, normal);
	}

	Plane::Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c) {

	}

	Plane::~Plane() {
	}

	bool Plane::Raycast(const Ray& r, float* enter) {
		float numer = Vector3d::DotProduct(mNormal, r.mOrigin) - d;
		float denom = Vector3d::DotProduct(mNormal, r.mDirection);

		if (Math::IsZero(denom)) { // normal is orthogonal to vector, cant intersect
			*enter = -1.f;
			return false;
		}
		*enter = -(numer / denom);
		return true;
	}
}