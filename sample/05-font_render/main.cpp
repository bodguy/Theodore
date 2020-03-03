// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "TheodoreEngine.h"

using namespace Theodore;

class MyFontRenderScene : public Scene {
public:
	MyFontRenderScene() : Scene("MyFontRenderScene") {}
	virtual ~MyFontRenderScene() override {}

	virtual void OnAwake() override {
		Font* fontAsset = AssetManager::RequestFont(Application::GetResourcePath() + "arial.ttf", 0, 48);
		GameObject* GUIFont = new GameObject("GUI font", this);
		FontRenderer* fr = GUIFont->AddComponent<FontRenderer>(fontAsset);
		fr->SetColor(Color::red);
		fr->SetText("AB");
		fr->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 0.f));
	}

	virtual void OnStart() override {
		Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
		Input::AddAxis("Forward", new InputHandler(KEY_Q, KEY_E, 0.01f));
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
		SceneManager::SetActiveScene(SceneManager::CreateScene<MyFontRenderScene>("MyFontRenderScene"));
		app.Run();
	}

	return 0;
}