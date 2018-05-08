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
	SpriteRenderer::SpriteRenderer() : Component("SpriteRenderer"), mSprite(nullptr), mColor(Color::white), mFlipX(false), mFlipY(false) {
		Shader* vs = AssetManager::RequestShader("Shaders/sprite/vs.glsl", Enumeration::VertexShader);
		Shader* fs = AssetManager::RequestShader("Shaders/sprite/fs.glsl", Enumeration::FragmentShader);
		mProgram = new Program();
		mProgram->AttachShader(*vs);
		mProgram->AttachShader(*fs);
		mProgram->Link();
	}

	SpriteRenderer::~SpriteRenderer() {
		SafeDealloc(mSprite);
		SafeDealloc(mProgram);
	}

	void SpriteRenderer::SetSprite(Sprite* sprite) {
		mSprite = sprite;
		mSprite->mVao->BindAttribute(mProgram->GetAttribute("position"), *mSprite->mVbo, 2, 2 * sizeof(Vector2d), 0);
		mSprite->mVao->BindAttribute(mProgram->GetAttribute("texcoord"), *mSprite->mVbo, 2, 2 * sizeof(Vector2d), sizeof(Vector2d));
		mSprite->mVao->BindElements(*mSprite->mEbo);
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
			Graphics::DrawElements(*mSprite->mVao, Enumeration::Primitive::Triangles, 0, 6, mSprite->mFormat);
			Graphics::BindTexture(0, NULL);
			mProgram->UnUse();

			/*
			// renew pivot value
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