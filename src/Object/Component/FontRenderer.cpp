// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "FontRenderer.h"

#include "Asset/Font.h"
#include "Asset/Shader.h"
#include "Graphics/Graphics.h"
#include "Graphics/VertexBuffer.h"
#include "Math/Vector4d.h"
#include "Object/Component/Camera.h"
#include "Object/SceneManager.h"
#include "Transform.h"

namespace Theodore {
  FontRenderer::FontRenderer() : Renderer("FontRenderer"), mFont(nullptr), mTextColor(Color()), mText(std::string()) {
    mPrimitive = Primitive::Triangles;
    mProgram = Shader::Find("Font");
  }

  FontRenderer::FontRenderer(Font* font) : Renderer("FontRenderer"), mFont(nullptr), mTextColor(Color()), mText(std::string()) {
    mProgram = Shader::Find("Font");
    SetFont(font);
  }

  FontRenderer::~FontRenderer() {}

  Font* FontRenderer::GetFont() const { return mFont; }

  Color FontRenderer::GetColor() const { return mTextColor; }

  std::string FontRenderer::GetText() const { return mText; }

  void FontRenderer::SetFont(Font* font) {
    mFont = font;
    //    Buffer* buffer = new Buffer(BufferType::BufferVertex);
    //    buffer->Data(nullptr, sizeof(Vector4d) * 6, BufferUsage::DynamicDraw);
    //    mVbos.push_back(buffer);
    //    mVao->BindAttribute(mProgram->GetAttribute("vertex"), *mVbos.front(), 4, sizeof(Vector4d), 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void FontRenderer::SetColor(const Color& color) { mTextColor = color; }

  void FontRenderer::SetText(const std::string& text) { mText = text; }

  void FontRenderer::Update(float deltaTime) {}

  void FontRenderer::Render() {
    mProgram->Use();
    mProgram->SetUniform("textColor", mTextColor);
    mProgram->SetUniform("projection", Matrix4x4::Orthogonal(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 1000.f));
    //    Graphics::Enable(Capabilities::Blending);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    Vector3d position = mTransform->GetPosition();
    Vector3d scale = Vector3d();
    scale.z = 0.1;
    for (auto& ch : mText) {
      GlyphInfo* glyph = mFont->FindGlyph(ch);
      if (glyph) {
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindTexture(static_cast<GLenum>(TextureDimension::Tex2D), glyph->texture_id);
        float xpos = position.x + glyph->bearingX * scale.z;
        float ypos = position.y - (glyph->bitmap.GetHeight() - glyph->bearingY) * scale.z;
        float w = glyph->bitmap.GetWidth() * scale.z;
        float h = glyph->bitmap.GetHeight() * scale.z;
        //     Vertex
        //	0----------1
        //  |          |
        //	3----------2
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},    {xpos, ypos, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 1.0},

            {xpos, ypos + h, 0.0, 0.0},    {xpos + w, ypos, 1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}
            //                Vector4d(x_pos, y_pos, 0.f, 1.f),
            //            Vector4d(x_pos + width, y_pos, 1.f, 1.f),
            //            Vector4d(x_pos + width, y_pos + height, 1.f, 0.f),
            //            Vector4d(x_pos, y_pos + height, 0.f, 0.f)
        };
        //        mVbos.front()->SubData(verts, 0, sizeof(Vector4d) * 6);
        //        Graphics::DrawArrays(*mVao, mPrimitive, 0, 6);

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, glyph->texture_id);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // calc next pen point
        position.x = glyph->advance * scale.z;
      }
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    //    Graphics::BindTexture(0, nullptr);
    mProgram->UnUse();
  }

  bool FontRenderer::CompareEquality(const Object& rhs) const {}

  bool FontRenderer::Destroy() {}
}  // namespace Theodore