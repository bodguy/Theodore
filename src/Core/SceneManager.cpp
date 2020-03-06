// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "SceneManager.h"
#include "GameObject.h"
#include "Helper/crc32.h"

namespace Theodore {
  SceneManager* SceneManager::instance = nullptr;
  SceneManager::SceneManager() : currentScene(nullptr), sceneCount(0), mainCamera(nullptr) {
    instance = this;
    scenes.clear();
  }

  SceneManager::~SceneManager() {
    for (auto& i : scenes) {
      if (i) {
        i->OnDestroy();
        i->~Scene();
        free(i);
        i = nullptr;
      }
    }
    scenes.clear();
  }

  SceneManager* SceneManager::GetInstance() { return instance; }

  Scene* SceneManager::GetActiveScene() { return GetInstance()->currentScene; }

  bool SceneManager::SetActiveScene(Scene* scene) {
    if (!scene) return false;

    GetInstance()->currentScene = scene;
    SetCurrentCamera();
    SetCurrentLight();
    scene->OnStart();

    return true;
  }

  Scene* SceneManager::GetSceneAt(uint32_t sceneBuildIndex) { return GetInstance()->scenes.at(sceneBuildIndex); }

  Scene* SceneManager::GetSceneByName(const std::string& sceneName) {
    uint32_t find = CRC32_STR(sceneName.c_str());
    for (auto& i : GetInstance()->scenes) {
      if (i->GetHashCode() == find) return i;
    }

    return nullptr;
  }

  bool SceneManager::LoadScene(uint32_t sceneBuildIndex) { return SetActiveScene(GetSceneAt(sceneBuildIndex)); }

  bool SceneManager::LoadScene(const std::string& sceneName) { return SetActiveScene(GetSceneByName(sceneName)); }

  uint32_t SceneManager::GetSceneCount() { return GetInstance()->sceneCount; }

  Camera* SceneManager::GetMainCamera() { return GetInstance()->mainCamera; }

  void SceneManager::SetMainCamera(Camera* cam) {
    if (cam) GetInstance()->mainCamera = cam;
  }

  void SceneManager::SetCurrentCamera() {
    if (GetActiveScene()) GetInstance()->mainCamera = GetActiveScene()->Find("MainCamera")->GetComponent<Camera>();
  }

  Light* SceneManager::GetGlobalLight() { return GetInstance()->globalLight; }

  void SceneManager::SetGlobalLight(Light* light) {
    if (light) GetInstance()->globalLight = light;
  }

  void SceneManager::SetCurrentLight() {
    if (GetActiveScene()) GetInstance()->globalLight = GetActiveScene()->Find("GlobalLight")->GetComponent<Light>();
  }

  void SceneManager::Update(float deltaTime) const {
    if (currentScene && currentScene->isActive) currentScene->Update(deltaTime);
  }

  void SceneManager::Render() const {
    if (currentScene && currentScene->isActive) currentScene->Render();
  }

  bool SceneManager::Remove(const std::string& sceneName) {
    uint32_t find = CRC32_STR(sceneName.c_str());
    for (std::vector<Scene*>::iterator iter = scenes.begin(); iter != scenes.end(); ++iter) {
      if ((*iter)->GetHashCode() == find) {
        (*iter)->OnDestroy();
        (*iter)->~Scene();
        free(*iter);
        (*iter) = nullptr;
        scenes.erase(iter);
        return true;
      }
    }

    return false;
  }
}  // namespace Theodore