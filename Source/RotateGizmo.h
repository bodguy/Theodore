#ifndef RotateGizmo_h
#define RotateGizmo_h

#include "Gizmo.h"
#include "VertexBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

namespace Quark {
	class RotateGizmo : public Gizmo {
	public:
		RotateGizmo();
		~RotateGizmo();

		virtual void Render() override;

	private:
		int mDegree;
		int mHalfCircleSegmentCount;
	};
}

#endif // RotateGizmo_h