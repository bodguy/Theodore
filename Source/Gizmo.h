#ifndef Gizmo_h
#define Gizmo_h

#include "VertexBuffer.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Component.h"
#include "Enumeration.h"

namespace Quark {
	class Gizmo : public Component {
	public:
		Gizmo(Enumeration::GizmoType type);
		~Gizmo();

		Transform& GetTransform();

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	protected:
		VertexArray* mVao;
		Transform mTransform;
		Program* mProgram;
		Enumeration::GizmoType mType;
	};
}

#endif // Gizmo_h