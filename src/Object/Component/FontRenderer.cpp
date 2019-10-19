// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "FontRenderer.h"

namespace Theodore {
    FontRenderer::FontRenderer() : Renderer("FontRenderer"), mFont(nullptr) {
        mPrimitive = Primitive::Triangles;
    }

    FontRenderer::FontRenderer(Font* font) : Renderer("FontRenderer"), mFont(font) {
        mPrimitive = Primitive::Triangles;
    }

    FontRenderer::~FontRenderer() {

    }

    void FontRenderer::SetFont(Font* font) {
        mFont = font;
    }

    void FontRenderer::Update(float deltaTime) {

    }

    void FontRenderer::Render() {

    }

    bool FontRenderer::CompareEquality(const Object& rhs) const {

    }

    bool FontRenderer::Destroy() {

    }
}