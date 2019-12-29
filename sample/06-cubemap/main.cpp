// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "TheodoreEngine.h"

using namespace Theodore;

class MyCubemapScene : public Scene {
public:
	MyCubemapScene() : Scene("MyCubemapScene") {}
	virtual ~MyCubemapScene() override {}

	virtual void OnAwake() override {
		SceneManager::GetMainCamera()->GetTransform()->Translate(Vector3d(0.f, 0.f, 10.f));

		GameObject* myCubemapObject = new GameObject("myCubemapObject", this);
		CubemapRenderer* cubemapRenderer = myCubemapObject->AddComponent<CubemapRenderer>();

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
		SceneManager::SetActiveScene(SceneManager::CreateScene<MyCubemapScene>("MyCubemapScene"));
		app.Run();
	}

	return 0;
}