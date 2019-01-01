/**
  @brief Gizmo type visual debugging
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Gizmo_h
#define Gizmo_h

#include "Component.h"
#include "Enumeration.h"

namespace Theodore {
  class VertexArray;
  class Pipeline;
  class Transform;
  class Gizmo : public Component {
  public:
    Gizmo(GizmoType type);
    virtual ~Gizmo();

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  protected:
    VertexArray* mVao;
    Pipeline* mProgram;
    GizmoType mType;
  };
}

#endif // Gizmo_h