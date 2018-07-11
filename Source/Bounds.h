#ifndef Bounds_h
#define Bounds_h

#include "Vector3d.h"
#include "Ray.h"

namespace Quark {
	class Bounds {
	public:
		Bounds(const Vector3d& center, const Vector3d& size);
		~Bounds(void);

		Vector3d GetMin() const;
		Vector3d GetMax() const;
		Vector3d GetCenter() const;
		Vector3d GetExtents() const;
		Vector3d GetSize() const;

		bool IntersectRay(const Ray& ray);
		bool Intersect(const Bounds& bounds);
		bool Contains(const Vector3d& point);
		void Encapsulate(const Vector3d& point);
		void Encapsulate(const Bounds& bounds);
		void ClosestPoint(const Vector3d& point);
		float SqrDistance(const Vector3d& point);
		void Expand(float amount);
		void SetMinMax(const Vector3d& min, const Vector3d& max);

	//private:
		// The minimal point of the box. This is always equal to center-extents.
		Vector3d mMin;
		// The maximal point of the box.This is always equal to center + extents.
		Vector3d mMax;
		// The center of the bounding box.
		Vector3d mCenter;
		// The extents of the Bounding Box. This is always half of the size of the Bounds.
		Vector3d mExtents;
		// The total size of the box. This is always twice as large as the extents.
		Vector3d mSize;
	};
}

#endif /* Bounds_h */