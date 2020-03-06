// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Sprite.h"

#include <cstring>

#include "Asset/AssetManager.h"
#include "Asset/Texture2D.h"

namespace Theodore {
  Sprite::Sprite()
      : indexFormat(IndexFormat::UInt16),
				texture(nullptr),
				rect(),
				textureRect(),
				textureRectOffset(),
				pivot(),
				initialPivot(),
				colorKey(Color::white),
				useColorKey(false),
				bounds(Vector3d::zero, Vector3d::one) {
    memset(vertices, 0, sizeof(Vector2d) * 4);
    memset(texcoords, 0, sizeof(Vector2d) * 4);
    memset(indices, 0, sizeof(unsigned short) * 6);
  }

  Sprite::~Sprite() {}

  Sprite* Sprite::Create(Texture2D* texture, const Rect<float> rect) {
    if (!texture || !texture->GetNativeTexturePtr()) return static_cast<Sprite*>(nullptr);

    Sprite* sprite = new Sprite();
    sprite->texture = texture;
    sprite->textureRect.left = 0.f;
    sprite->textureRect.top = 0.f;
    sprite->textureRect.right = static_cast<float>(sprite->texture->GetWidth());
    sprite->textureRect.bottom = static_cast<float>(sprite->texture->GetHeight());
    if (rect == Rect<float>::zero) {
      const_cast<Rect<float>&>(rect).right = sprite->textureRect.right;
      const_cast<Rect<float>&>(rect).bottom = sprite->textureRect.bottom;
    }
    sprite->textureRectOffset.x = sprite->textureRect.right * 0.5f;   // (rect.right - rect.left) / sprite->mTextureRect.right;
    sprite->textureRectOffset.y = sprite->textureRect.bottom * 0.5f;  // (rect.bottom - rect.top) / sprite->mTextureRect.bottom;

    //	   Vertex		  Texture
    //	0----------1	0----------1
    //	|          |	|          |
    //	3----------2	3----------2

    // set local vertex position
    sprite->vertices[0] = Vector2d(-sprite->textureRectOffset.x, sprite->textureRectOffset.y);
    sprite->vertices[1] = Vector2d(sprite->textureRectOffset.x, sprite->textureRectOffset.y);
    sprite->vertices[2] = Vector2d(sprite->textureRectOffset.x, -sprite->textureRectOffset.y);
    sprite->vertices[3] = Vector2d(-sprite->textureRectOffset.x, -sprite->textureRectOffset.y);

    // set local texture uvs
    sprite->rect.top = rect.top / sprite->textureRect.bottom;
    sprite->rect.bottom = rect.bottom / sprite->textureRect.bottom;
    sprite->rect.left = rect.left / sprite->textureRect.right;
    sprite->rect.right = rect.right / sprite->textureRect.right;

    sprite->texcoords[0] = Vector2d(sprite->rect.left, sprite->rect.top);
    sprite->texcoords[1] = Vector2d(sprite->rect.right, sprite->rect.top);
    sprite->texcoords[2] = Vector2d(sprite->rect.right, sprite->rect.bottom);
    sprite->texcoords[3] = Vector2d(sprite->rect.left, sprite->rect.bottom);

    sprite->colorKey = texture->GetColorKey();
    sprite->useColorKey = texture->UseColorKey();

    sprite->indices[0] = 0;
    sprite->indices[1] = 1;
    sprite->indices[2] = 2;
    sprite->indices[3] = 0;
    sprite->indices[4] = 2;
    sprite->indices[5] = 3;

    return sprite;
  }

  void Sprite::RecalculateBounds() {
    std::vector<Vector3d>::const_iterator iter;
    Vector3d min, max;

    for (int i = 0; i < 4; i++) {
      if (vertices[i].x < min.x) {
        min.x = vertices[i].x;
      } else if (vertices[i].x > max.x) {
        max.x = vertices[i].x;
      }

      if (vertices[i].y < min.y) {
        min.y = vertices[i].y;
      } else if (vertices[i].y > max.y) {
        max.y = vertices[i].y;
      }
    }

    bounds.SetMinMax(min, max);
  }

  bool Sprite::operator==(const Sprite& rhs) {
    return false;  // TODO: modify
  }

  bool Sprite::operator!=(const Sprite& rhs) { return !(*this == rhs); }
}  // namespace Theodore