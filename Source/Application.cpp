#include "Application.h"
#include "Input.h"
#include "Time.h"
#include "SceneManager.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "Random.h"
#include "Utility.h"
#include "Shader.h"
#include "Debug.h"

namespace Theodore {
	Application* Application::instance = nullptr;
	std::string Application::dataPath = "./Contents/";
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
		Graphics::Dispose();
	}

	bool Application::Initialize(const PlatformContext& param) {
		mPlatform = new Platform();
		if (!mPlatform->Initialize(param)) {
			return false;
		}
		mPlatform->SetVSync(true);

		mInput = new Input();
		mTime = new Time();
		mAssetManager = new AssetManager();
		mShaderManager = new ShaderManager();

		// default program setting and caching
		Shader* phong_vs = AssetManager::RequestShader("Shaders/light/phong.vs", ShaderType::VertexShader);
		Shader* phong_fs = AssetManager::RequestShader("Shaders/light/phong.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Pipeline("Phong", *phong_vs, *phong_fs));

		//Shader* flat_vs = AssetManager::RequestShader("Shaders/light/flat.vs", ShaderType::VertexShader);
		//Shader* flat_fs = AssetManager::RequestShader("Shaders/light/flat.fs", ShaderType::FragmentShader);
		//ShaderManager::Append(new Pipeline("Flat", *flat_vs, *flat_fs));

		Shader* twoDimension_vs = AssetManager::RequestShader("Shaders/sprite/sprite.vs", ShaderType::VertexShader);
		Shader* twoDimension_fs = AssetManager::RequestShader("Shaders/sprite/sprite.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Pipeline("2D", *twoDimension_vs, *twoDimension_fs));

		Shader* gizmo_vs = AssetManager::RequestShader("Shaders/gizmo/gizmo.vs", ShaderType::VertexShader);
		Shader* gizmo_fs = AssetManager::RequestShader("Shaders/gizmo/gizmo.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Pipeline("Gizmo", *gizmo_vs, *gizmo_fs));

		Shader* sphere_gs = AssetManager::RequestShader("Shaders/gizmo/sphere.gs", ShaderType::GeometryShader);
		ShaderManager::Append(new Pipeline("Sphere", *gizmo_vs, *gizmo_fs, *sphere_gs));

		Shader* normal_vs = AssetManager::RequestShader("Shaders/debug/normal.vs", ShaderType::VertexShader);
		Shader* normal_gs = AssetManager::RequestShader("Shaders/debug/normal.gs", ShaderType::GeometryShader);
		Shader* normal_fs = AssetManager::RequestShader("Shaders/debug/normal.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Pipeline("DebugNormal", *normal_vs, *normal_fs, *normal_gs));

		Shader* shadow_vs = AssetManager::RequestShader("Shaders/light/shadow.vs", ShaderType::VertexShader);
		Shader* shadow_fs = AssetManager::RequestShader("Shaders/light/shadow.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Pipeline("Shadow", *shadow_vs, *shadow_fs));

		Shader* cubemap_vs = AssetManager::RequestShader("Shaders/cubemap/cubemap.vs", ShaderType::VertexShader);
		Shader* cubemap_fs = AssetManager::RequestShader("Shaders/cubemap/cubemap.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Pipeline("Cubemap", *cubemap_vs, *cubemap_fs));

		mSceneManager = new SceneManager();
		Graphics::SetGraphicsSettings();
		Graphics::Enable(Capabilities::DepthTest);

		Random::InitState(static_cast<int>(time(NULL)));

		return true;
	}

	void Application::Run() {
		while (mPlatform->IsRunning()) {
			mPlatform->Update();
			mTime->Update();
			mInput->Update();

			Graphics::ClearColor(Color(0.f, 0.f, 0.f, 1.f), BufferBits::ColorBits | BufferBits::DepthBits);
			{
				// TODO FixedUpdate codes here
				Update(Time::DeltaTime());
				Render();
			}
			mPlatform->SwapBuffer();
		}
	}

	std::string Application::GetDataPath() {
		return Application::dataPath;
	}

	void Application::Render() {
		mSceneManager->Render();
	}

	void Application::Update(float deltaTime) {
		mSceneManager->Update(deltaTime);
	}
}
