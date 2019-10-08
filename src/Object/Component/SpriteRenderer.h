// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "Math/Color.h"
#include "Renderer.h"

namespace Theodore {
  class Sprite;
  class SpriteRenderer : public Renderer {
  public:
    SpriteRenderer();
    virtual ~SpriteRenderer();

    void SetSprite(Sprite* sprite);
    void SetColor(const Color& color);
    void SetFlipX(const bool flipX);
    void SetFlipY(const bool flipY);
    Color GetColor() const;
    bool GetFlipX() const;
    bool GetFlipY() const;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  private:
    Sprite* mSprite;
    Color mColor;
    bool mFlipX, mFlipY;
  };
}  // namespace Theodore

#endif /* SpriteRenderer_h */