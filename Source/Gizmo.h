#ifndef Gizmo_h
#define Gizmo_h

#include "Component.h"
#include "Enumeration.h"

namespace Quark {
	class VertexArray; class Program; class Transform;
	class Gizmo : public Component {
	public:
		Gizmo(GizmoType type);
		virtual ~Gizmo();

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	protected:
		VertexArray* mVao;
		Program* mProgram;
		Transform* mTransform;
		GizmoType mType;
	};
}

#endif // Gizmo_h