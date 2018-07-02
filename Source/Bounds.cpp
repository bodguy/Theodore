#include "Bounds.h"

namespace Quark {
	Bounds::Bounds(const Vector3d& center, const Vector3d& size) :center(center), size(size) {
		extents = size / 2.f;
		min = center - extents;
		max = center + extents;
	}

	Bounds::~Bounds(void) {
	}

	bool Bounds::IntersectRay(const Ray& rat) {
		return false;
	}

	bool Bounds::Intersect(const Bounds& bounds) {
		if (max < bounds.min || min > bounds.max) return false;
		return true;
	}

	bool Bounds::Contains(const Vector3d& point) {
		return false;
	}

	void Bounds::Encapsulate(const Vector3d& point) {

	}

	void Bounds::Encapsulate(const Bounds& bounds) {

	}

	void Bounds::ClosestPoint(const Vector3d& point) {

	}

	float Bounds::SqrDistance(const Vector3d& point) {
		return 0.f;
	}

	void Bounds::Expand(float amount) {

	}

	void Bounds::SetMinMax(const Vector3d& pmin, const Vector3d& pmax) {
		min = pmin;
		max = pmax;
		center = (max + min) * 0.5f;
		extents = center - min;
		size = extents * 2.f;
	}
}