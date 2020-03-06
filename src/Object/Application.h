// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Application_h
#define Application_h

#include <string>

#include "Graphics/Enumeration.h"
#include "Platform/Platform.h"

namespace Theodore {
  class Platform;
  class Input;
  class Time;
  class SceneManager;
  class AssetManager;
  class ShaderManager;
  class Application {
  public:
    Application();
    ~Application();

    bool Initialize(const PlatformContext& param);
    void Run();
    static std::string GetResourcePath();
    static std::string GetShaderPath();

  private:
    static Application* instance;

    void Render();
    void Update(float deltaTime);

    Platform* platform;
    Input* input;
    Time* timer;
    SceneManager* sceneManager;
    AssetManager* assetManager;
    ShaderManager* shaderManager;
    static std::string resourcePath;
    static std::string shaderPath;
  };
}  // namespace Theodore

#endif /* Application_h */