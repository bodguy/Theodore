// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Gizmo_h
#define Gizmo_h

#include "Component.h"
#include "Graphics/Enumeration.h"

namespace Theodore {
  class VertexArray;
  class Pipeline;
  class Transform;
  class Gizmo : public Component {
  public:
    Gizmo(GizmoType type);
    virtual ~Gizmo() override;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  protected:
    VertexArray* vertexArray;
    Pipeline* pipeline;
    GizmoType gizmoType;
  };
}  // namespace Theodore

#endif  // Gizmo_h