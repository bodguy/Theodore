#include "Bounds.h"
#include "Debug.h"
#include "Vector4d.h"
#include <cmath>

namespace Theodore {
	Bounds::Bounds(const Vector3d& center, const Vector3d& size) :mCenter(center), mSize(size) {
		mExtents = size / 2.f;
		mMin = center - mExtents;
		mMax = center + mExtents;
	}

	Bounds::~Bounds(void) {
	}

	Vector3d Bounds::GetMin() const {
		return mMin;
	}

	Vector3d Bounds::GetMax() const {
		return mMax;
	}

	Vector3d Bounds::GetCenter() const {
		return mCenter;
	}

	Vector3d Bounds::GetExtents() const {
		return mExtents;
	}

	Vector3d Bounds::GetSize() const {
		return mSize;
	}

	bool Bounds::IntersectRay(const Ray& ray) {
		float t1 = (mMin[0] - ray.origin[0]) * ray.invDirection[0];
		float t2 = (mMax[0] - ray.origin[0]) * ray.invDirection[0];

		float tmin = std::fminf(t1, t2);
		float tmax = std::fmaxf(t1, t2);

		for (unsigned int i = 1; i < 3; i++) {
			t1 = (mMin[i] - ray.origin[i]) * ray.invDirection[i];
			t2 = (mMax[i] - ray.origin[i]) * ray.invDirection[i];

			tmin = std::fmaxf(tmin, std::fminf(t1, t2));
			tmax = std::fminf(tmax, std::fmaxf(t1, t2));
		}

		return tmax > std::fmaxf(tmin, 0.f);
	}

	bool Bounds::Intersect(const Bounds& bounds) {
		if (mMax.x < bounds.mMin.x || mMin.x > bounds.mMax.x) return false;
		if (mMax.y < bounds.mMin.y || mMin.y > bounds.mMax.y) return false;
		if (mMax.z < bounds.mMin.z || mMin.z > bounds.mMax.z) return false;

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

	void Bounds::SetMinMax(const Vector3d& min, const Vector3d& max) {
		mMin = min;
		mMax = max;
		mCenter = (mMax + mMin) * 0.5f;
		mExtents = mCenter - mMin;
		mSize = mExtents * 2.f;
	}
}