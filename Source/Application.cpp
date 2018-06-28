#include "Application.h"
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
		ShaderManager::Append(new Program("Phong", *phong_vs, *phong_fs));

		Shader* flat_vs = AssetManager::RequestShader("Shaders/light/flat.vs", ShaderType::VertexShader);
		Shader* flat_fs = AssetManager::RequestShader("Shaders/light/flat.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Program("Flat", *flat_vs, *flat_fs));

		Shader* twoDimension_vs = AssetManager::RequestShader("Shaders/sprite/vs.glsl", ShaderType::VertexShader);
		Shader* twoDimension_fs = AssetManager::RequestShader("Shaders/sprite/fs.glsl", ShaderType::FragmentShader);
		ShaderManager::Append(new Program("2D", *twoDimension_vs, *twoDimension_fs));

		Shader* gizmo_vs = AssetManager::RequestShader("Shaders/gizmo/vs.glsl", ShaderType::VertexShader);
		Shader* gizmo_fs = AssetManager::RequestShader("Shaders/gizmo/fs.glsl", ShaderType::FragmentShader);
		ShaderManager::Append(new Program("Gizmo", *gizmo_vs, *gizmo_fs));

		Shader* debug_vs1 = AssetManager::RequestShader("Shaders/debug/visualize_normals/vs.glsl", ShaderType::VertexShader);
		Shader* debug_gs1 = AssetManager::RequestShader("Shaders/debug/visualize_normals/gs.glsl", ShaderType::GeometryShader);
		Shader* debug_fs1 = AssetManager::RequestShader("Shaders/debug/visualize_normals/fs.glsl", ShaderType::FragmentShader);
		ShaderManager::Append(new Program("DebugNormal", *debug_vs1, *debug_fs1, *debug_gs1));

		Shader* shadow_vs = AssetManager::RequestShader("Shaders/light/shadow_vs.glsl", ShaderType::VertexShader);
		Shader* shadow_fs = AssetManager::RequestShader("Shaders/light/shadow_fs.glsl", ShaderType::FragmentShader);
		ShaderManager::Append(new Program("Shadow", *shadow_vs, *shadow_fs));

		Shader* cubemap_vs = AssetManager::RequestShader("Shaders/cubemap/cubemap.vs", ShaderType::VertexShader);
		Shader* cubemap_fs = AssetManager::RequestShader("Shaders/cubemap/cubemap.fs", ShaderType::FragmentShader);
		ShaderManager::Append(new Program("Cubemap", *cubemap_vs, *cubemap_fs));

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
			Update(Time::DeltaTime());
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
