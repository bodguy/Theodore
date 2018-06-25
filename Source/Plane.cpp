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
		d = -Vector3d::DotProduct(inPoint, mNormal);
	}

	Plane::Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c) {

	}

	Plane::~Plane() {
	}

	bool Plane::Raycast(const Ray& r, float* enter) {
		float numer = Vector3d::DotProduct(r.Origin, mNormal) + d;
		float denom = Vector3d::DotProduct(r.Direction, mNormal);

		if (denom < Math::flt_epsilon) { // normal is orthogonal to vector, cant intersect
			*enter = -1.f;
			return false;
		}
		*enter = -(numer / denom);
		return true;
	}
}