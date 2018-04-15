#ifndef ScaleGizmo_h
#define ScaleGizmo_h

#include "Gizmo.h"
#include "VertexBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

namespace Quark {
	class ScaleGizmo : public Gizmo {
	public:
		ScaleGizmo();
		virtual ~ScaleGizmo();

		virtual void Render(const Camera& cam) override;
	};
}

#endif // ScaleGizmo_h