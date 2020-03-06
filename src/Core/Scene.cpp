// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Scene.h"

#include <algorithm>

#include "Component/Camera.h"
#include "Component/Light.h"
#include "Component/Transform.h"
#include "GameObject.h"
#include "Graphics/Graphics.h"
#include "Helper/Utility.h"
#include "Helper/crc32.h"
#include "SceneManager.h"

namespace Theodore {
  Scene::Scene(const std::string& name) : Object(name), isActive(true) {
    /* Does not initialize SceneManager, BuildIndex here! */
    gameObjects.clear();
    // Every scene has at least a camera.
    GameObject* camera = new GameObject("MainCamera", this);
    Camera* cameraComponent = camera->AddComponent<Camera>();
		sceneManager->mMainCamera = cameraComponent;

    // global light source setting.
    GameObject* lightSource = new GameObject("GlobalLight", this);
    Light* lightComponent = lightSource->AddComponent<Light>(LightType::DirectionalLight);
    lightComponent->GetTransform()->SetLocalPosition(Vector3d(10.f, 10.f, 10.f));
		sceneManager->mGlobalLight = lightComponent;
  }

  Scene::~Scene() {
    for (auto& i : gameObjects) {
      SafeDealloc(i);
    }
    gameObjects.clear();
  }

  bool Scene::IsActive() const { return isActive; }

  void Scene::SetActive(bool value) {
    value ? OnEnable() : OnDisable();
		isActive = value;
  }

  void Scene::Attach(GameObject* object) {
    if (object == nullptr) return;
    object->scene = this;

    gameObjects.push_back(object);
  }

  GameObject* Scene::Find(const std::string& name) const {
    uint32_t find = CRC32_STR(name.c_str());
    for (auto& i : gameObjects) {
      if (i->GetHashCode() == find) return i;
    }

    return nullptr;
  }

  GameObject* Scene::FindWithTag(const std::string& tag) const {
    uint32_t value = CRC32_STR(tag.c_str());
    for (auto& i : gameObjects) {
      if (i->tag == value) return i;
    }

    return nullptr;
  }

  std::vector<GameObject*> Scene::FindGameObjectsWithTag(const std::string& tag) const {
    std::vector<GameObject*> vectors;
    uint32_t value = CRC32_STR(tag.c_str());
    for (auto& i : gameObjects) {
      if (i->tag == value) vectors.push_back(i);
    }

    return std::move(vectors);
  }

  void Scene::Update(float deltaTime) {
    // std::sort(sprites.begin(), sprites.end(), SpriteRenderer::SortLayer);
    for (auto& i : gameObjects)
      if (i->activeSelf) i->Update(deltaTime);
    OnUpdate();
    OnLateUpdate();
  }

  void Scene::Render() {
    OnPreRender();

    for (auto& i : gameObjects)
      if (i->activeSelf) i->Render();

    OnAfterRender();
  }

  bool Scene::CompareEquality(const Object& rhs) const {
    bool returnValue = false;
    const Scene* temp_rhs = dynamic_cast<const Scene*>(&rhs);
    if (temp_rhs) {
      // compare
      auto leftIt = gameObjects.begin();
      auto rightIt = temp_rhs->gameObjects.begin();
      while (leftIt != gameObjects.end() && rightIt != temp_rhs->gameObjects.end()) {
        if (*leftIt != *rightIt) {
          returnValue = false;
          return returnValue;
        }
        ++leftIt;
        ++rightIt;
      }
    }

    return returnValue;
  }

  bool Scene::Destroy() {
    for (auto& i : gameObjects) {
      SafeDealloc(i);
    }
    gameObjects.clear();
    return sceneManager->Remove(this->ToString());
  }

  bool Scene::Remove(GameObject* object) {
    auto iter = std::find(gameObjects.begin(), gameObjects.end(), object);
    if (iter == gameObjects.end()) return false;

    SafeDealloc(*iter);
    gameObjects.erase(iter);

    return true;
  }
}  // namespace Theodore
