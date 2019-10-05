/**
  @brief renderer component, every rendering component must inherit this class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Renderer_h
#define Renderer_h

#include <vector>
#include "../../Geometry/Bounds.h"
#include "../../Graphics/Enumeration.h"
#include "Component.h"

namespace Theodore {
  class Buffer;
  class VertexArray;
  class Pipeline;
  class Renderer : public Component {
  public:
    Renderer(const std::string& name);
    virtual ~Renderer();

    void SetPrimitive(const Primitive primitive);
    bool GetVisibleGizmos() const;
    void SetVisibleGizmos(bool visible);
    Bounds* GetBounds();

  protected:
    std::vector<Buffer*> mVbos;
    std::vector<Buffer*> mEbos;
    VertexArray* mVao;
    Pipeline* mProgram;
    Pipeline* mNormalVisualizeProgram;
    Primitive mPrimitive;
    // world space bounds
    Bounds mBounds;
    bool mIsVisibleGizmos;
  };
}  // namespace Theodore

#endif /* Renderer_h */