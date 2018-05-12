#ifndef Plane_h
#define Plane_h

#include "Vector3d.h"
#include "Ray.h"

namespace Quark {
	class Plane {
	public:
		Plane(Vector3d inNormal, const Vector3d& inPoint);
		Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
		~Plane();

		bool Raycast(const Ray& r, float* enter);

	private:
		Vector3d mNormal;
		float d;
	};
}

#endif // Plane_h