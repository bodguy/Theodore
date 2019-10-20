// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef FontRenderer_h
#define FontRenderer_h

#include <string>
#include "Math/Color.h"
#include "Renderer.h"

namespace Theodore {
  class Font;
  class FontRenderer : public Renderer {
  public:
    FontRenderer();
    FontRenderer(Font* font);
    ~FontRenderer();

    Font* GetFont() const;
    Color GetColor() const;
    std::string GetText() const;
    float GetScale() const;
    void SetFont(Font* font);
    void SetColor(const Color& color);
    void SetText(const std::string& text);
    void SetScale(float scale);

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  private:
    Font* mFont;
    Color mTextColor;
    std::string mText;
    float mScale;
  };
}  // namespace Theodore

#endif  // FontRenderer_h
