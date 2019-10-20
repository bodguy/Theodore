// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "FontRenderer.h"
#include "Asset/Font.h"
#include "Asset/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/VertexBuffer.h"
#include "Math/Vector2d.h"
#include "Object/Component/Camera.h"
#include "Object/SceneManager.h"
#include "Transform.h"

namespace Theodore {
  FontRenderer::FontRenderer() : Renderer("FontRenderer"), mFont(nullptr), mTextColor(Color()), mText(std::string()), mScale(0.f) {
    mPrimitive = Primitive::Triangles;
    mProgram = Shader::Find("Font");
  }

  FontRenderer::FontRenderer(Font* font) : Renderer("FontRenderer"), mFont(nullptr), mTextColor(Color()), mText(std::string()), mScale(0.f) {
    mProgram = Shader::Find("Font");
    SetFont(font);
  }

  FontRenderer::~FontRenderer() {}

  Font* FontRenderer::GetFont() const { return mFont; }

  Color FontRenderer::GetColor() const { return mTextColor; }

  std::string FontRenderer::GetText() const { return mText; }

  float FontRenderer::GetScale() const { return mScale; }

  void FontRenderer::SetFont(Font* font) {
    mFont = font;

    Buffer* buffer = new Buffer(BufferType::BufferVertex);
    buffer->Data(nullptr, sizeof(Vector2d) * 8, BufferUsage::DynamicDraw);  // vertex position data is dynamic
    //     Texture
    //	0----------1
    //  |          |
    //	3----------2
    Vector2d uvs[4] = {Vector2d(0.f, 1.f), Vector2d(1.f, 1.f), Vector2d(1.f, 0.f), Vector2d(0.f, 0.f)};
    buffer->SubData(uvs, sizeof(Vector2d) * 4, sizeof(Vector2d) * 4);
    mVbos.push_back(buffer);

    Buffer* index = new Buffer(BufferType::BufferIndex);
    unsigned short indices[6] = {0, 1, 2, 0, 2, 3};
    index->Data(indices, sizeof(unsigned short) * 6, BufferUsage::DynamicDraw);
    mEbos.push_back(index);

    mVao->BindAttribute(mProgram->GetAttribute("position"), *mVbos.front(), 2, sizeof(Vector2d), 0);
    mVao->BindAttribute(mProgram->GetAttribute("texcoord"), *mVbos.front(), 2, sizeof(Vector2d), sizeof(Vector2d) * 4);
    mVao->BindElements(*mEbos.front());
  }

  void FontRenderer::SetColor(const Color& color) { mTextColor = color; }

  void FontRenderer::SetText(const std::string& text) { mText = text; }

  void FontRenderer::SetScale(float scale) { mScale = scale; }

  void FontRenderer::Update(float deltaTime) {}

  void FontRenderer::Render() {
    mProgram->Use();
    mProgram->SetUniform("textColor", mTextColor);
    mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    Graphics::Enable(Capabilities::Blending);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Vector3d position = mTransform->GetPosition();
    for (auto& ctor : mText) {
      GlyphInfo* glyph = mFont->FindGlyph(ctor);
      if (glyph) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(static_cast<GLenum>(TextureDimension::Tex2D), glyph->texture_id);
        float x_pos = position.x + glyph->bearingX * mScale;
        float y_pos = position.y - (glyph->bitmap.GetHeight() - glyph->bearingY) * mScale;
        float width = glyph->bitmap.GetWidth() * mScale;
        float height = glyph->bitmap.GetHeight() * mScale;
        // VBO position subdata
        //     Vertex
        //	0----------1
        //  |          |
        //	3----------2
        Vector2d vertices_pos[4] = {Vector2d(x_pos, y_pos), Vector2d(x_pos + width, y_pos), Vector2d(x_pos + width, y_pos + height), Vector2d(x_pos, y_pos + height)};
        mVbos.front()->SubData(vertices_pos, 0, sizeof(Vector2d) * 4);
        Graphics::DrawElements(*mVao, mPrimitive, 0, 6, IndexFormat::UInt16);
        // calc next pen point
        position.x = glyph->advance * mScale;
      }
    }

    Graphics::BindTexture(0, nullptr);
    mProgram->UnUse();
  }

  bool FontRenderer::CompareEquality(const Object& rhs) const {}

  bool FontRenderer::Destroy() {}
}  // namespace Theodore