/**
  @brief entrypoint class that assembles all the necessary codes
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Application_h
#define Application_h

#include "Enumeration.h"
#include "Platform.h"
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
    std::string GetDataPath();

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
    static std::string dataPath;
  };
}

#endif /* Application_h */