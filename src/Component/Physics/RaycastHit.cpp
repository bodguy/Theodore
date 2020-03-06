// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "RaycastHit.h"

namespace Theodore {
	RaycastHit::RaycastHit() : transform(nullptr), collider(nullptr), point(), normal(), distance(0.f) {

	}

	RaycastHit::~RaycastHit() {

	}
}
