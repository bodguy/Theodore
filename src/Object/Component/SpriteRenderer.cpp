#include "SpriteRenderer.h"
#include "AssetManager.h"
#include "Camera.h"
#include "GameObject.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "Transform.h"
#include "Utility.h"

namespace Theodore {
  SpriteRenderer::SpriteRenderer()
      : Renderer("SpriteRenderer"), mSprite(nullptr), mColor(Color::white), mFlipX(false),
        mFlipY(false) {
    mProgram = Shader::Find("2D");
    mPrimitive = Primitive::Triangles;
  }

  SpriteRenderer::~SpriteRenderer() { SafeDealloc(mSprite); }

  void SpriteRenderer::SetSprite(Sprite* sprite) {
    mSprite = sprite;

    Buffer* buffer = new Buffer(BufferType::BufferVertex);
    buffer->Data(nullptr, sizeof(Vector2d) * 8, BufferUsage::StaticDraw);
    buffer->SubData(mSprite->mVertices, 0, sizeof(Vector2d) * 4);
    buffer->SubData(mSprite->mUvs, sizeof(Vector2d) * 4, sizeof(Vector2d) * 4);
    mVbos.push_back(buffer);

    Buffer* index = new Buffer(BufferType::BufferIndex);
    index->Data(mSprite->mIndices, sizeof(unsigned short) * 6, BufferUsage::StaticDraw);
    mEbos.push_back(index);

    mVao->BindAttribute(mProgram->GetAttribute("position"), *mVbos.front(), 2, sizeof(Vector2d), 0);
    mVao->BindAttribute(mProgram->GetAttribute("texcoord"), *mVbos.front(), 2, sizeof(Vector2d),
                        sizeof(Vector2d) * 4);
    mVao->BindElements(*mEbos.front());

    mSprite->RecalculateBounds();
    mBounds.SetMinMax(mSprite->mBounds.GetMin(), mSprite->mBounds.GetMax());
    // setup initial sprite pivot(center)
    mSprite->mInitialPivot =
        Vector2d((mSprite->mBounds.GetMin() + mSprite->mBounds.GetMax()) / 2.f);
  }

  void SpriteRenderer::SetColor(const Color& color) { mColor = color; }

  void SpriteRenderer::SetFlipX(const bool flipX) { mFlipX = flipX; }

  void SpriteRenderer::SetFlipY(const bool flipY) { mFlipY = flipY; }

  Color SpriteRenderer::GetColor() const { return mColor; }

  bool SpriteRenderer::GetFlipX() const { return mFlipX; }

  bool SpriteRenderer::GetFlipY() const { return mFlipY; }

  void SpriteRenderer::Update(float deltaTime) {
    Matrix4x4 world = mTransform->GetLocalToWorldMatrix();
    Matrix4x4 model = mTransform->GetWorldToLocalMatrix();

    Vector3d center = mSprite->mBounds.GetCenter();
    Vector3d extents = mSprite->mBounds.GetExtents();

    Vector2d decompsedTranslation = Vector2d(Matrix4x4::DecomposeTranslation(world));
    Vector2d decomposedScale = Vector2d(Matrix4x4::DecomposeScale(world));
    Vector2d powScale = Math::Pow(decomposedScale, 2.f);

    Vector2d newCenter = decompsedTranslation +
                         powScale * Vector2d(Math::Dot(Vector3d(model.rows[0]), center),
                                             Math::Dot(Vector3d(model.rows[1]), center));

    Vector2d newExtents = powScale * Vector2d(Math::AbsDot(Vector3d(model.rows[0]), extents),
                                              Math::AbsDot(Vector3d(model.rows[1]), extents));

    // update bounds min, max every frame
    mBounds.SetMinMax(Vector3d(newCenter - newExtents), Vector3d(newCenter + newExtents));
    // update pivot every frame (TODO)
    mSprite->mPivot = decompsedTranslation + (mSprite->mInitialPivot * decomposedScale); // / 2.f;
  }

  void SpriteRenderer::Render() {
    if (mSprite) {
      Graphics::DrawCube(mBounds.GetCenter(), mBounds.GetSize(), Color::red);
      Graphics::DrawLine(Vector3d(0.f, 0.f, 10.f), Vector3d(mSprite->mPivot), Color::red);

      Graphics::Enable(Capabilities::Blending);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      Graphics::Enable(Capabilities::AlphaTest);
      glAlphaFunc(GL_GREATER, 0);

      mProgram->Use();
      mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
      mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
      mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
      mProgram->SetUniform("flipX", mFlipX);
      mProgram->SetUniform("flipY", mFlipY);
      mProgram->SetUniform("color", mColor);
      mProgram->SetUniform("colorKey", mSprite->mColorKey);
      mProgram->SetUniform("useColorKey", mSprite->mUseColorKey);
      Graphics::BindTexture(0, mSprite->mTexture);
      Graphics::DrawElements(*mVao, mPrimitive, 0, 6, mSprite->mFormat);
      Graphics::BindTexture(0, NULL);
      mProgram->UnUse();
    }
  }

  bool SpriteRenderer::CompareEquality(const Object& rhs) const {
    const SpriteRenderer* target = dynamic_cast<const SpriteRenderer*>(&rhs);
    if (target) {
      return (mFlipX == target->mFlipX && mFlipY == target->mFlipY &&
              *(const_cast<Color*>(&(mColor))) == target->mColor && *mSprite == *(target->mSprite));
    }

    return false;
  }

  bool SpriteRenderer::Destroy() { return mGameObject->RemoveComponent<SpriteRenderer>(); }
}