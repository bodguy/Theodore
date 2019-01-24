#include "Application.h"
#include "../Asset/AssetManager.h"
#include "../Helper/Debug.h"
#include "../Graphics/Graphics.h"
#include "../Platform/Input.h"
#include "../Math/Random.h"
#include "SceneManager.h"
#include "../Asset/Shader.h"
#include "../Platform/Time.h"
#include "../Helper/Utility.h"

namespace Theodore {
  Application* Application::instance = nullptr;
  std::string Application::ResourcePath = "Resources/";
	std::string Application::ShaderPath = "Shaders/";
  Application::Application() { instance = this; }

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
    Shader* phong_vs =
        AssetManager::RequestShader(Application::GetShaderPath() + "light/phong.vs", ShaderType::VertexShader);
    Shader* phong_fs =
        AssetManager::RequestShader(Application::GetShaderPath() + "light/phong.fs", ShaderType::FragmentShader);
    ShaderManager::Append(new Pipeline("Phong", *phong_vs, *phong_fs));

    // Shader* flat_vs = AssetManager::RequestShader(Application::GetShaderPath() + "light/flat.vs",
    // ShaderType::VertexShader);
    // Shader* flat_fs = AssetManager::RequestShader(Application::GetShaderPath() + "light/flat.fs",
    // ShaderType::FragmentShader);
    // ShaderManager::Append(new Pipeline("Flat", *flat_vs, *flat_fs));

    Shader* twoDimension_vs =
        AssetManager::RequestShader(Application::GetShaderPath() + "sprite/sprite.vs", ShaderType::VertexShader);
    Shader* twoDimension_fs =
        AssetManager::RequestShader(Application::GetShaderPath() + "sprite/sprite.fs", ShaderType::FragmentShader);
    ShaderManager::Append(new Pipeline("2D", *twoDimension_vs, *twoDimension_fs));

    Shader* gizmo_vs =
        AssetManager::RequestShader(Application::GetShaderPath() + "gizmo/gizmo.vs", ShaderType::VertexShader);
    Shader* gizmo_fs =
        AssetManager::RequestShader(Application::GetShaderPath() + "gizmo/gizmo.fs", ShaderType::FragmentShader);
    ShaderManager::Append(new Pipeline("Gizmo", *gizmo_vs, *gizmo_fs));

    Shader* sphere_gs =
        AssetManager::RequestShader(Application::GetShaderPath() + "gizmo/sphere.gs", ShaderType::GeometryShader);
    ShaderManager::Append(new Pipeline("Sphere", *gizmo_vs, *gizmo_fs, *sphere_gs));

    Shader* normal_vs =
        AssetManager::RequestShader(Application::GetShaderPath() + "debug/normal.vs", ShaderType::VertexShader);
    Shader* normal_gs =
        AssetManager::RequestShader(Application::GetShaderPath() + "debug/normal.gs", ShaderType::GeometryShader);
    Shader* normal_fs =
        AssetManager::RequestShader(Application::GetShaderPath() + "debug/normal.fs", ShaderType::FragmentShader);
    ShaderManager::Append(new Pipeline("DebugNormal", *normal_vs, *normal_fs, *normal_gs));

    Shader* shadow_vs =
        AssetManager::RequestShader(Application::GetShaderPath() + "light/shadow.vs", ShaderType::VertexShader);
    Shader* shadow_fs =
        AssetManager::RequestShader(Application::GetShaderPath() + "light/shadow.fs", ShaderType::FragmentShader);
    ShaderManager::Append(new Pipeline("Shadow", *shadow_vs, *shadow_fs));

    Shader* cubemap_vs =
        AssetManager::RequestShader(Application::GetShaderPath() + "cubemap/cubemap.vs", ShaderType::VertexShader);
    Shader* cubemap_fs =
        AssetManager::RequestShader(Application::GetShaderPath() + "cubemap/cubemap.fs", ShaderType::FragmentShader);
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

      Graphics::ClearColor(Color(0.f, 0.f, 0.f, 1.f),
                           BufferBits::ColorBits | BufferBits::DepthBits);
      {
        // TODO FixedUpdate codes here
        Update(Time::DeltaTime());
        Render();
      }
      mPlatform->SwapBuffer();
    }
  }

  std::string Application::GetResourcePath() { return Application::ResourcePath; }

	std::string Application::GetShaderPath() { return Application::ShaderPath; }

  void Application::Render() { mSceneManager->Render(); }

  void Application::Update(float deltaTime) { mSceneManager->Update(deltaTime); }
}
