/**
  @brief sprite datastructure for sprite rendering
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Sprite_h
#define Sprite_h

#include "../../../Geometry/Bounds.h"
#include "../../../Graphics/Enumeration.h"
#include "../../../Graphics/VertexBuffer.h"
#include "../../../Math/Color.h"
#include "../../../Math/Rect.h"
#include "../../../Math/Vector2d.h"

namespace Theodore {
  class Texture2D;
  class Buffer;
  class VertexArray;
  class Sprite {
    friend class SpriteRenderer;

  public:
    Sprite();
    ~Sprite();

    static Sprite* Create(Texture2D* texture, const Rect rect = Rect::zero);
    void RecalculateBounds();
    bool operator==(const Sprite& rhs);
    bool operator!=(const Sprite& rhs);

  private:
    IndexFormat mFormat;
    Texture2D* mTexture;
    Rect mRect;         // for a sprite with a single texture
    Rect mTextureRect;  // for a sprite from an atlas
    Vector2d mTextureRectOffset;
    Vector2d mPivot;
    Vector2d mInitialPivot;
    Vector2d mVertices[4];
    Vector2d mUvs[4];
    Color mColorKey;
    bool mUseColorKey;
    unsigned short mIndices[6];
    Bounds mBounds;  // local space bounds
  };
}  // namespace Theodore

#endif /* Sprite_h */