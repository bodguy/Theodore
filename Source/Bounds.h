#ifndef Bounds_h
#define Bounds_h

#include "Vector3d.h"
#include "Ray.h"

namespace Quark {
	class Bounds {
	public:
		Bounds(const Vector3d& center, const Vector3d& size);
		~Bounds(void);

		bool IntersectRay(const Ray& rat);
		bool Intersect(const Bounds& bounds);
		bool Contains(const Vector3d& point);
		void Encapsulate(const Vector3d& point);
		void Encapsulate(const Bounds& bounds);
		void ClosestPoint(const Vector3d& point);
		float SqrDistance(const Vector3d& point);
		void Expand(float amount);
		void SetMinMax(const Vector3d& pmin, const Vector3d& pmax);

		// The minimal point of the box. This is always equal to center-extents.
		Vector3d min;
		// The maximal point of the box.This is always equal to center + extents.
		Vector3d max;
		// The center of the bounding box.
		Vector3d center;
		// The extents of the Bounding Box. This is always half of the size of the Bounds.
		Vector3d extents;
		// The total size of the box. This is always twice as large as the extents.
		Vector3d size;
	};
}

#endif /* Bounds_h */