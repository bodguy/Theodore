#ifndef TranslateGizmo_h
#define TranslateGizmo_h

#include "Gizmo.h"
#include "VertexBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

namespace Quark {
	class TranslateGizmo : public Gizmo {
	public:
		TranslateGizmo();
		virtual ~TranslateGizmo();

		virtual void Render() override;
	};
}

#endif // TranslateGizmo_h