// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "SceneManager.h"

#include "GameObject.h"
#include "Helper/crc32.h"

namespace Theodore {
  SceneManager* SceneManager::instance = nullptr;
  SceneManager::SceneManager() : mCurrentScene(nullptr), mSceneCount(0), mMainCamera(nullptr) {
    instance = this;
    mScenes.clear();
  }

  SceneManager::~SceneManager() {
    for (auto& i : mScenes) {
      if (i) {
        i->OnDestroy();
        i->~Scene();
        free(i);
        i = nullptr;
      }
    }
    mScenes.clear();
  }

  SceneManager* SceneManager::GetInstance() { return instance; }

  Scene* SceneManager::GetActiveScene() { return GetInstance()->mCurrentScene; }

  bool SceneManager::SetActiveScene(Scene* scene) {
    if (!scene) return false;

    GetInstance()->mCurrentScene = scene;
    SetCurrentCamera();
    SetCurrentLight();
    scene->OnStart();

    return true;
  }

  Scene* SceneManager::GetSceneAt(uint32_t sceneBuildIndex) { return GetInstance()->mScenes.at(sceneBuildIndex); }

  Scene* SceneManager::GetSceneByName(const std::string& sceneName) {
    uint32_t find = CRC32_STR(sceneName.c_str());
    for (auto& i : GetInstance()->mScenes) {
      if (i->GetHashCode() == find) return i;
    }

    return nullptr;
  }

  bool SceneManager::LoadScene(uint32_t sceneBuildIndex) { return SetActiveScene(GetSceneAt(sceneBuildIndex)); }

  bool SceneManager::LoadScene(const std::string& sceneName) { return SetActiveScene(GetSceneByName(sceneName)); }

  uint32_t SceneManager::GetSceneCount() { return GetInstance()->mSceneCount; }

  Camera* SceneManager::GetMainCamera() { return GetInstance()->mMainCamera; }

  void SceneManager::SetMainCamera(Camera* cam) {
    if (cam) GetInstance()->mMainCamera = cam;
  }

  void SceneManager::SetCurrentCamera() {
    if (GetActiveScene()) GetInstance()->mMainCamera = GetActiveScene()->Find("MainCamera")->GetComponent<Camera>();
  }

  Light* SceneManager::GetGlobalLight() { return GetInstance()->mGlobalLight; }

  void SceneManager::SetGlobalLight(Light* light) {
    if (light) GetInstance()->mGlobalLight = light;
  }

  void SceneManager::SetCurrentLight() {
    if (GetActiveScene()) GetInstance()->mGlobalLight = GetActiveScene()->Find("GlobalLight")->GetComponent<Light>();
  }

  void SceneManager::Update(float deltaTime) const {
    if (mCurrentScene && mCurrentScene->mActive) mCurrentScene->Update(deltaTime);
  }

  void SceneManager::Render() const {
    if (mCurrentScene && mCurrentScene->mActive) mCurrentScene->Render();
  }

  bool SceneManager::Remove(const std::string& sceneName) {
    uint32_t find = CRC32_STR(sceneName.c_str());
    for (std::vector<Scene*>::iterator iter = mScenes.begin(); iter != mScenes.end(); ++iter) {
      if ((*iter)->GetHashCode() == find) {
        (*iter)->OnDestroy();
        (*iter)->~Scene();
        free(*iter);
        (*iter) = nullptr;
        mScenes.erase(iter);
        return true;
      }
    }

    return false;
  }
}  // namespace Theodore