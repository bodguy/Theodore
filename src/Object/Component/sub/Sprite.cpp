#include "Sprite.h"
#include "../../../Asset/AssetManager.h"
#include "../../../Asset/Texture2D.h"

namespace Theodore {
  Sprite::Sprite()
      : mFormat(IndexFormat::UInt16),
        mTexture(nullptr),
        mRect(),
        mTextureRect(),
        mTextureRectOffset(),
        mPivot(),
        mInitialPivot(),
        mColorKey(Color::white),
        mUseColorKey(false),
        mBounds(Vector3d::zero, Vector3d::one) {
    memset(mVertices, 0, sizeof(Vector2d) * 4);
    memset(mUvs, 0, sizeof(Vector2d) * 4);
    memset(mIndices, 0, sizeof(unsigned short) * 6);
  }

  Sprite::~Sprite() {}

  Sprite* Sprite::Create(Texture2D* texture, const Rect rect) {
    if (!texture || !texture->GetNativeTexturePtr()) return static_cast<Sprite*>(nullptr);

    Sprite* sprite = new Sprite();
    sprite->mTexture = texture;
    sprite->mTextureRect.left = 0.f;
    sprite->mTextureRect.top = 0.f;
    sprite->mTextureRect.right = static_cast<float>(sprite->mTexture->GetWidth());
    sprite->mTextureRect.bottom = static_cast<float>(sprite->mTexture->GetHeight());
    if (rect == Rect::zero) {
      const_cast<Rect&>(rect).right = sprite->mTextureRect.right;
      const_cast<Rect&>(rect).bottom = sprite->mTextureRect.bottom;
    }
    sprite->mTextureRectOffset.x = sprite->mTextureRect.right * 0.5f;   // (rect.right - rect.left) / sprite->mTextureRect.right;
    sprite->mTextureRectOffset.y = sprite->mTextureRect.bottom * 0.5f;  // (rect.bottom - rect.top) / sprite->mTextureRect.bottom;

    //	   Vertex		  Texture
    //	0----------1	0----------1
    //	|          |	|          |
    //	3----------2	3----------2

    // set local vertex position
    sprite->mVertices[0] = Vector2d(-sprite->mTextureRectOffset.x, sprite->mTextureRectOffset.y);
    sprite->mVertices[1] = Vector2d(sprite->mTextureRectOffset.x, sprite->mTextureRectOffset.y);
    sprite->mVertices[2] = Vector2d(sprite->mTextureRectOffset.x, -sprite->mTextureRectOffset.y);
    sprite->mVertices[3] = Vector2d(-sprite->mTextureRectOffset.x, -sprite->mTextureRectOffset.y);

    // set local texture uvs
    sprite->mRect.top = rect.top / sprite->mTextureRect.bottom;
    sprite->mRect.bottom = rect.bottom / sprite->mTextureRect.bottom;
    sprite->mRect.left = rect.left / sprite->mTextureRect.right;
    sprite->mRect.right = rect.right / sprite->mTextureRect.right;

    sprite->mUvs[0] = Vector2d(sprite->mRect.left, sprite->mRect.top);
    sprite->mUvs[1] = Vector2d(sprite->mRect.right, sprite->mRect.top);
    sprite->mUvs[2] = Vector2d(sprite->mRect.right, sprite->mRect.bottom);
    sprite->mUvs[3] = Vector2d(sprite->mRect.left, sprite->mRect.bottom);

    sprite->mColorKey = texture->GetColorKey();
    sprite->mUseColorKey = texture->UseColorKey();

    sprite->mIndices[0] = 0;
    sprite->mIndices[1] = 1;
    sprite->mIndices[2] = 2;
    sprite->mIndices[3] = 0;
    sprite->mIndices[4] = 2;
    sprite->mIndices[5] = 3;

    return sprite;
  }

  void Sprite::RecalculateBounds() {
    std::vector<Vector3d>::const_iterator iter;
    Vector3d min, max;

    for (int i = 0; i < 4; i++) {
      if (mVertices[i].x < min.x) {
        min.x = mVertices[i].x;
      } else if (mVertices[i].x > max.x) {
        max.x = mVertices[i].x;
      }

      if (mVertices[i].y < min.y) {
        min.y = mVertices[i].y;
      } else if (mVertices[i].y > max.y) {
        max.y = mVertices[i].y;
      }
    }

    mBounds.SetMinMax(min, max);
  }

  bool Sprite::operator==(const Sprite& rhs) {
    return false;  // TODO: modify
  }

  bool Sprite::operator!=(const Sprite& rhs) { return !(*this == rhs); }
}  // namespace Theodore