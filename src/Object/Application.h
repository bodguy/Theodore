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

    Platform* mPlatform;
    Input* mInput;
    Time* mTime;
    SceneManager* mSceneManager;
    AssetManager* mAssetManager;
    ShaderManager* mShaderManager;
    static std::string ResourcePath;
    static std::string ShaderPath;
  };
}  // namespace Theodore

#endif /* Application_h */