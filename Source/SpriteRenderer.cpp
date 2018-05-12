#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Utility.h"
#include "Sprite.h"
#include "Shader.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Texture2D.h"

namespace Quark {
	SpriteRenderer::SpriteRenderer() : Renderer("SpriteRenderer"), mSprite(nullptr), mColor(Color::white), mFlipX(false), mFlipY(false) {
		mProgram = Shader::Find("2D");
	}

	SpriteRenderer::~SpriteRenderer() {
		SafeDealloc(mSprite);
	}

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
		mVao->BindAttribute(mProgram->GetAttribute("texcoord"), *mVbos.front(), 2, sizeof(Vector2d), sizeof(Vector2d) * 4);
		mVao->BindElements(*mEbos.front());
	}

	void SpriteRenderer::SetColor(const Color& color) {
		mColor = color;
	}

	void SpriteRenderer::SetFlipX(const bool flipX) {
		mFlipX = flipX;
	}

	void SpriteRenderer::SetFlipY(const bool flipY) {
		mFlipY = flipY;
	}

	Color SpriteRenderer::GetColor() const {
		return mColor;
	}

	bool SpriteRenderer::GetFlipX() const {
		return mFlipX;
	}
	
	bool SpriteRenderer::GetFlipY() const {
		return mFlipY;
	}

	void SpriteRenderer::Update(double deltaTime) {
	}

	void SpriteRenderer::Render() {
		if (mSprite) {
			mProgram->Use();
			mProgram->SetUniform(mProgram->GetUniform("model"), mGameObject->GetTransform()->GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("flipX"), mFlipX);
			mProgram->SetUniform(mProgram->GetUniform("flipY"), mFlipY);
			mProgram->SetUniform(mProgram->GetUniform("color"), mColor);
			Graphics::BindTexture(0, mSprite->mTexture);
			Graphics::DrawElements(*mVao, Primitive::Triangles, 0, 6, mSprite->mFormat);
			Graphics::BindTexture(0, NULL);
			mProgram->UnUse();

			/*
			mSprite->mPivot = Vector3d(
				mGameObject->mTransform->mPosition.x + (mSprite->mTextureRectOffset.x * mGameObject->mTransform->mScale.x) / 2.f,
				mGameObject->mTransform->mPosition.y + (mSprite->mTextureRectOffset.y * mGameObject->mTransform->mScale.y) / 2.f);
			*/
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

	bool SpriteRenderer::Destroy() {
		return mGameObject->RemoveComponent<SpriteRenderer>();
	}
}