// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Vertex_h
#define Vertex_h

#include "Math/Vector3d.h"

namespace Theodore {
	class Vertex {
	public:
		Vertex() : position(), texcoord(), normal(), tangent() {}

		Vector3d position;
		Vector2d texcoord;
		Vector3d normal;
		Vector3d tangent;
	};
} // namespace Theodore

#endif /* Vertex_h */
