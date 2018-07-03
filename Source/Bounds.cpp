#include "Bounds.h"
#include <cmath>

namespace Quark {
	Bounds::Bounds(const Vector3d& center, const Vector3d& size) :center(center), size(size) {
		extents = size / 2.f;
		min = center - extents;
		max = center + extents;
	}

	Bounds::~Bounds(void) {
	}

	bool Bounds::IntersectRay(const Ray& ray) {
		float t1 = (min[0] - ray.origin[0]) * ray.invDirection[0];
		float t2 = (max[0] - ray.origin[0]) * ray.invDirection[0];

		float tmin = std::fminf(t1, t2);
		float tmax = std::fmaxf(t1, t2);

		for (unsigned int i = 1; i < 3; i++) {
			t1 = (min[i] - ray.origin[i]) * ray.invDirection[i];
			t2 = (max[i] - ray.origin[i]) * ray.invDirection[i];

			tmin = std::fmaxf(tmin, std::fminf(t1, t2));
			tmax = std::fminf(tmax, std::fmaxf(t1, t2));
		}

		return tmax > std::fmaxf(tmin, 0.f);
	}

	bool Bounds::Intersect(const Bounds& bounds) {
		if (max.x < bounds.min.x || min.x > bounds.max.x) return false;
		if (max.y < bounds.min.y || min.y > bounds.max.y) return false;
		if (max.z < bounds.min.z || min.z > bounds.max.z) return false;

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