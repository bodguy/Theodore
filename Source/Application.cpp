#include "Application.h"
#include "Platform.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "Random.h"
#include "Utility.h"

namespace Quark {
	Application* Application::instance = nullptr;
	Application::Application() {
		instance = this;
	}

	Application::~Application() {
		SafeDealloc(mAssetManager);
		SafeDealloc(mSceneManager);
		SafeDealloc(mTime);
		SafeDealloc(mInput);
		SafeDealloc(mPlatform);
	}

	bool Application::Initialize(const std::string& name, int width, int height, bool fullmode, int majorVersion, int minorVersion, int multisample, Enumeration::WindowStyle style) {
		mPlatform = new Platform();
		if (!mPlatform->Initialize(name, width, height, fullmode, majorVersion, minorVersion, multisample, style)) {
			return false;
		}
		mPlatform->SetVSync(true);
		Graphics::Enable(Enumeration::DepthTest);

		mInput = new Input();
		mTime = new Time();
		mAssetManager = new AssetManager();
		mSceneManager = new SceneManager();

		Random::InitState(static_cast<int>(time(NULL)));

		return true;
	}

	void Application::Run() {
		while (mPlatform->IsRunning()) {
			mPlatform->Update();
			mTime->Update();
			mInput->Update();
			Update(Time::DeltaTime());
			Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), Enumeration::ColorBits | Enumeration::DepthBits);
			Render();
			mPlatform->SwapBuffer();
		}
	}

	void Application::Render() {
		mSceneManager->Render();
	}

	void Application::Update(double deltaTime) {
		mSceneManager->Update(deltaTime);
	}
}
