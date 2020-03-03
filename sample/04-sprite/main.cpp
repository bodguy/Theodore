// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "TheodoreEngine.h"

using namespace Theodore;

class MySpriteScene : public Scene {
public:
	MySpriteScene() : Scene("MySpriteScene") {}
	virtual ~MySpriteScene() override {}

	virtual void OnAwake() override {
		SceneManager::GetMainCamera()->GetTransform()->Translate(Vector3d(0.f, 0.f, 10.f));

		GameObject* mySpriteObject = new GameObject("mySpriteObject", this);
		SpriteRenderer* spriteRenderer = mySpriteObject->AddComponent<SpriteRenderer>();
		Sprite* spriteSheet = Sprite::Create(AssetManager::RequestTexture(Application::GetResourcePath() + "dragon.png", TextureFormat::RGBA32));
		spriteRenderer->SetSprite(spriteSheet);
		mySpriteObject->GetTransform()->SetLocalScale(Vector3d(0.1f, 0.1f, 0.1f));
		spriteRenderer->SetColor(Color(0.f, 1.f, 0.f, 1.f));
		spriteRenderer->SetFlipX(true);
		spriteRenderer->SetFlipY(true);
	}

	virtual void OnStart() override {
		Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
	}

	virtual void OnUpdate() override {
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}
	}
};

int main(int argc, char** argv) {
	Application app;
	PlatformContext context;
	context.width = 1280;
	context.height = 720;

	if (app.Initialize(context)) {
		SceneManager::SetActiveScene(SceneManager::CreateScene<MySpriteScene>("MySpriteScene"));
		app.Run();
	}

	return 0;
}