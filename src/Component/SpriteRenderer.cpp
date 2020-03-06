// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "SpriteRenderer.h"

#include "Asset/AssetManager.h"
#include "Asset/Shader.h"
#include "Asset/Texture2D.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Helper/Utility.h"
#include "Sprite.h"
#include "Object/GameObject.h"
#include "Object/SceneManager.h"
#include "Transform.h"

namespace Theodore {
  SpriteRenderer::SpriteRenderer() : Renderer("SpriteRenderer"), sprite(nullptr), color(Color::white), flipX(false), flipY(false) {
		pipeline = Shader::Find("2D");
		primitive = Primitive::Triangles;
  }

  SpriteRenderer::~SpriteRenderer() { SafeDealloc(sprite); }

  void SpriteRenderer::SetSprite(Sprite* sprite) {
    sprite = sprite;

    Buffer* buffer = new Buffer(BufferType::BufferVertex);
    buffer->Data(nullptr, sizeof(Vector2d) * 8, BufferUsage::StaticDraw);
    buffer->SubData(sprite->vertices, 0, sizeof(Vector2d) * 4);
    buffer->SubData(sprite->texcoords, sizeof(Vector2d) * 4, sizeof(Vector2d) * 4);
    vertexBuffers.push_back(buffer);

    Buffer* index = new Buffer(BufferType::BufferIndex);
    index->Data(sprite->indices, sizeof(unsigned short) * 6, BufferUsage::StaticDraw);
    indexBuffers.push_back(index);

    vertexArray->BindAttribute(pipeline->GetAttribute("position"), *vertexBuffers.front(), 2, sizeof(Vector2d), 0);
    vertexArray->BindAttribute(pipeline->GetAttribute("texcoord"), *vertexBuffers.front(), 2, sizeof(Vector2d), sizeof(Vector2d) * 4);
    vertexArray->BindElements(*indexBuffers.front());

    sprite->RecalculateBounds();
    bounds.SetMinMax(sprite->bounds.GetMin(), sprite->bounds.GetMax());
    // setup initial sprite pivot(center)
    sprite->initialPivot = Vector2d((sprite->bounds.GetMin() + sprite->bounds.GetMax()) / 2.f);
  }

  void SpriteRenderer::SetColor(const Color& color) { color = color; }

  void SpriteRenderer::SetFlipX(const bool flipX) { flipX = flipX; }

  void SpriteRenderer::SetFlipY(const bool flipY) { flipY = flipY; }

  Color SpriteRenderer::GetColor() const { return color; }

  bool SpriteRenderer::GetFlipX() const { return flipX; }

  bool SpriteRenderer::GetFlipY() const { return flipY; }

  void SpriteRenderer::Update(float deltaTime) {
    Matrix4x4 world = transform->GetLocalToWorldMatrix();
    Matrix4x4 model = transform->GetWorldToLocalMatrix();

    Vector3d center = sprite->bounds.GetCenter();
    Vector3d extents = sprite->bounds.GetExtents();

    Vector2d decompsedTranslation = Vector2d(Matrix4x4::DecomposeTranslation(world));
    Vector2d decomposedScale = Vector2d(Matrix4x4::DecomposeScale(world));
    Vector2d powScale = Math::Pow(decomposedScale, 2.f);

    Vector2d newCenter = decompsedTranslation + powScale * Vector2d(Math::Dot(Vector3d(model.rows[0]), center), Math::Dot(Vector3d(model.rows[1]), center));

    Vector2d newExtents = powScale * Vector2d(Math::AbsDot(Vector3d(model.rows[0]), extents), Math::AbsDot(Vector3d(model.rows[1]), extents));

    // update bounds min, max every frame
    bounds.SetMinMax(Vector3d(newCenter - newExtents), Vector3d(newCenter + newExtents));
    // update pivot every frame (TODO)
    sprite->pivot = decompsedTranslation + (sprite->initialPivot * decomposedScale);  // / 2.f;
  }

  void SpriteRenderer::Render() {
    if (sprite) {
      Graphics::DrawCube(bounds.GetCenter(), bounds.GetSize(), Color::red);
      Graphics::DrawLine(Vector3d(0.f, 0.f, 10.f), Vector3d(sprite->pivot), Color::red);

      Graphics::Enable(Capabilities::Blending);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      Graphics::Enable(Capabilities::AlphaTest);
      glAlphaFunc(GL_GREATER, 0);

      pipeline->Use();
      pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
      pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
      pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
      pipeline->SetUniform("flipX", flipX);
      pipeline->SetUniform("flipY", flipY);
      pipeline->SetUniform("color", color);
      pipeline->SetUniform("colorKey", sprite->colorKey);
      pipeline->SetUniform("useColorKey", sprite->useColorKey);
      Graphics::BindTexture(0, sprite->texture);
      Graphics::DrawElements(*vertexArray, primitive, 0, 6, sprite->indexFormat);
      Graphics::BindTexture(0, NULL);
      pipeline->UnUse();
      Graphics::Disable(Capabilities::AlphaTest);
      Graphics::Disable(Capabilities::Blending);
    }
  }

  bool SpriteRenderer::CompareEquality(const Object& rhs) const {
    const SpriteRenderer* target = dynamic_cast<const SpriteRenderer*>(&rhs);
    if (target) {
      return (flipX == target->flipX && flipY == target->flipY && *(const_cast<Color*>(&(color))) == target->color && *sprite == *(target->sprite));
    }

    return false;
  }

  bool SpriteRenderer::Destroy() { return gameObject->RemoveComponent<SpriteRenderer>(); }
}  // namespace Theodore