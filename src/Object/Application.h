/**
  @brief entrypoint class that assembles all the necessary codes
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Application_h
#define Application_h

#include "../Graphics/Enumeration.h"
#include "../Platform/Platform.h"
#include <string>

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
}

#endif /* Application_h */