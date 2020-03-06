// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef SceneManager_h
#define SceneManager_h

#include <vector>
#include "Scene.h"

namespace Theodore {
  class Camera;
  class Scene;
  class Light;
  class SceneManager {
    friend class Scene;

  public:
    SceneManager();
    ~SceneManager();

    static SceneManager* GetInstance();
    template <typename T, typename... Ts>
    static Scene* CreateScene(const std::string& sceneName, Ts... args);
    static Scene* GetActiveScene();
    static bool SetActiveScene(Scene* scene);
    static Scene* GetSceneAt(uint32_t sceneBuildIndex);
    static Scene* GetSceneByName(const std::string& sceneName);
    static bool LoadScene(uint32_t sceneBuildIndex);
    static bool LoadScene(const std::string& sceneName);
    static uint32_t GetSceneCount();
    static Camera* GetMainCamera();
    static void SetMainCamera(Camera* cam);
    static void SetCurrentCamera();
    static Light* GetGlobalLight();
    static void SetGlobalLight(Light* light);
    static void SetCurrentLight();

    void Update(float deltaTime) const;
    void Render() const;

  private:
    bool Remove(const std::string& sceneName);

    std::vector<Scene*> scenes;
    Scene* currentScene;
    uint32_t sceneCount;
    Camera* mainCamera;
    Light* globalLight;
    static SceneManager* instance;
  };

  template <typename T, typename... Ts>
  Scene* SceneManager::CreateScene(const std::string& sceneName, Ts... args) {
    // find already exist scene in mScenes.
    if (GetSceneByName(sceneName)) return nullptr;

    // if there is not scene named sceneName here, and then build scene.
    // Notice that scene has to initialized with malloc because of the SceneManager member that
    // scene class has been.
    // reminder, cause scene instance is allocated with malloc(), the instance of scene has to be
    // dealloc with free()
    // and deinitialized with calling destructor manually.
    Scene* scene = static_cast<T*>(malloc(sizeof(T)));
    scene->sceneManager = GetInstance();
    // assign the scene count that it will be stored.
    scene->buildIndex = scene->sceneManager->sceneCount;
    // then, call c++ constructor with placement new operator.
    new (scene) T(args...);                   // parameterize variadic constructor members.
    scene->OnAwake();                         // call awake function before stored it
    GetInstance()->scenes.push_back(scene);   // store new build scene in array.
    GetInstance()->sceneCount++;              // increment scene counter.
    SetCurrentCamera();                       // restore to current scene main camera as the scena manager main camera.
    SetCurrentLight();                        // retore to current scene global light source.

    // succesfully initialized the scene then return it.
    return scene;
  }
}  // namespace Theodore
#endif /* SceneManager_h */
