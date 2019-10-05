/**
  @brief skinned mesh renderer component
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef SkinnedMeshRenderer_h
#define SkinnedMeshRenderer_h

#include "Renderer.h"

namespace Theodore {
  class SkinnedMeshRenderer : public Renderer {
  public:
    SkinnedMeshRenderer();
    virtual ~SkinnedMeshRenderer();

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;
  };
}  // namespace Theodore

#endif /* SkinnedMeshRenderer_h */