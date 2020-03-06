// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef RaycastHit_h
#define RaycastHit_h

#include "Math/Vector3d.h"

namespace Theodore {
	class Transform;
	class Collider;
	class RaycastHit {
	public:
		RaycastHit();
		~RaycastHit();

	private:
		Transform* transform;
		Collider* collider;
		Vector3d point;
		Vector3d normal;
		float distance;
	};
}  // namespace Theodore


#endif /* RaycastHit_h */
