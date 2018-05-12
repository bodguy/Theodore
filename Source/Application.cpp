#include "Application.h"
#include "Platform.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "Random.h"
#include "Utility.h"
#include "Shader.h"

namespace Quark {
	Application* Application::instance = nullptr;
	Application::Application() {
		instance = this;
	}

	Application::~Application() {
		SafeDealloc(mSceneManager);
		SafeDealloc(mShaderManager);
		SafeDealloc(mAssetManager);
		SafeDealloc(mTime);
		SafeDealloc(mInput);
		SafeDealloc(mPlatform);
	}

	bool Application::Initialize(const std::string& name, int width, int height, bool fullmode, int majorVersion, int minorVersion, int multisample, WindowStyle style) {
		mPlatform = new Platform();
		if (!mPlatform->Initialize(name, width, height, fullmode, majorVersion, minorVersion, multisample, style)) {
			return false;
		}
		mPlatform->SetVSync(true);
		Graphics::Enable(Capabilities::DepthTest);

		mInput = new Input();
		mTime = new Time();
		mAssetManager = new AssetManager();
		mShaderManager = new ShaderManager();

		// default program setting and caching
		Shader* standard_vs = AssetManager::RequestShader("Shaders/light/vs.glsl", ShaderType::VertexShader);
		Shader* standard_fs = AssetManager::RequestShader("Shaders/light/fs.glsl", ShaderType::FragmentShader);
		Program* standardProgram = new Program("Standard", *standard_vs, *standard_fs);
		ShaderManager::Append(standardProgram);

		Shader* twoDimension_vs = AssetManager::RequestShader("Shaders/sprite/vs.glsl", ShaderType::VertexShader);
		Shader* twoDimension_fs = AssetManager::RequestShader("Shaders/sprite/fs.glsl", ShaderType::FragmentShader);
		Program* twoDimensionProgram = new Program("2D", *twoDimension_vs, *twoDimension_fs);
		ShaderManager::Append(twoDimensionProgram);

		Shader* gizmo_vs = AssetManager::RequestShader("Shaders/gizmo/vs.glsl", ShaderType::VertexShader);
		Shader* gizmo_fs = AssetManager::RequestShader("Shaders/gizmo/fs.glsl", ShaderType::FragmentShader);
		Program* gizmoProgram = new Program("Gizmo", *gizmo_vs, *gizmo_fs);
		ShaderManager::Append(gizmoProgram);

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
			Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), BufferBits::ColorBits | BufferBits::DepthBits);
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
