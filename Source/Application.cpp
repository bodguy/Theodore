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
	}

	bool Application::Initialize(const PlatformContext& param) {
		mPlatform = new Platform();
		if (!mPlatform->Initialize(param)) {
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

		Shader* debug_vs1 = AssetManager::RequestShader("Shaders/debug/visualize_normals/vs.glsl", ShaderType::VertexShader);
		Shader* debug_gs1 = AssetManager::RequestShader("Shaders/debug/visualize_normals/gs.glsl", ShaderType::GeometryShader);
		Shader* debug_fs1 = AssetManager::RequestShader("Shaders/debug/visualize_normals/fs.glsl", ShaderType::FragmentShader);
		Program* debug_program1 = new Program("DebugNormal", *debug_vs1, *debug_fs1, *debug_gs1);
		ShaderManager::Append(debug_program1);

		Shader* shadow_vs = AssetManager::RequestShader("Shaders/light/shadow_vs.glsl", ShaderType::VertexShader);
		Shader* shadow_fs = AssetManager::RequestShader("Shaders/light/shadow_fs.glsl", ShaderType::FragmentShader);
		Program* shadow_program = new Program("Shadow", *shadow_vs, *shadow_fs);
		ShaderManager::Append(shadow_program);

		Shader* cubemap_vs = AssetManager::RequestShader("Shaders/cubemap/cubemap_vs.glsl", ShaderType::VertexShader);
		Shader* cubemap_fs = AssetManager::RequestShader("Shaders/cubemap/cubemap_fs.glsl", ShaderType::FragmentShader);
		Program* cubemap_program = new Program("Cubemap", *cubemap_vs, *cubemap_fs);
		ShaderManager::Append(cubemap_program);

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
			Graphics::ClearColor(Color(0.f, 0.f, 0.f, 1.f), BufferBits::ColorBits | BufferBits::DepthBits);
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
