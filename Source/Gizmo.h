#ifndef Gizmo_h
#define Gizmo_h

#include "VertexBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

namespace Quark {
	class Gizmo {
	public:
		Gizmo();
		~Gizmo();

		virtual void Render() = 0;
		Transform& GetTransform();

	protected:
		VertexArray mVao;
		Transform mTransform;
		Program mProgram;
	};
}

#endif // Gizmo_h