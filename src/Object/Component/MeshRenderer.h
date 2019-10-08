// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef MeshRenderer_h
#define MeshRenderer_h

#include "Renderer.h"

namespace Theodore {
  class Material;
  class Mesh;
  class Pipeline;
  class MeshRenderer : public Renderer {
  public:
    MeshRenderer();
    virtual ~MeshRenderer();

    void SetMaterial(Material* mat);
    void SetMesh(Mesh* mesh);
    Mesh* GetMesh() const;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

    void InternalRender();

  protected:
    Material* mMaterial;
    Mesh* mMesh;
  };
}  // namespace Theodore

#endif /* MeshRenderer_h */