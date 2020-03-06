// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Sprite_h
#define Sprite_h

#include "Geometry/Bounds.h"
#include "Graphics/Enumeration.h"
#include "Graphics/VertexBuffer.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "Math/Vector2d.h"

namespace Theodore {
  class Texture2D;
  class Buffer;
  class VertexArray;
  class Sprite {
    friend class SpriteRenderer;

  public:
    Sprite();
    ~Sprite();

    static Sprite* Create(Texture2D* texture, const Rect<float> rect = Rect<float>::zero);
    void RecalculateBounds();
    bool operator==(const Sprite& rhs);
    bool operator!=(const Sprite& rhs);

  private:
    IndexFormat indexFormat;
    Texture2D* texture;
    Rect<float> rect;         // for a sprite with a single texture
    Rect<float> textureRect;  // for a sprite from an atlas
    Vector2d textureRectOffset;
    Vector2d pivot;
    Vector2d initialPivot;
    Vector2d vertices[4];
    Vector2d texcoords[4];
    Color colorKey;
    bool useColorKey;
    unsigned short indices[6];
    Bounds bounds;  // local space bounds
  };
}  // namespace Theodore

#endif /* Sprite_h */