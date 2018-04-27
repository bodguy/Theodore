#ifndef Bounds_h
#define Bounds_h

#include "Vector3d.h"

namespace Quark {
	class Bounds {
	public:
		Bounds(const Vector3d& center, const Vector3d& size);
		~Bounds(void);

	private:
		Vector3d mMin;
		Vector3d mMax;
		Vector3d mCenter;
		Vector3d mExtents;
		Vector3d mSize;
	};
}

#endif /* Bounds_h */