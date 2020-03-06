// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Scene_h
#define Scene_h

#include <vector>

#include "Object.h"

namespace Theodore {
  class SceneManager;
  class GameObject;
  class Light;
  class Camera;
  class Collider;
  class Scene : public Object {
    friend class SceneManager;
    friend class GameObject;

  public:
    explicit Scene(const std::string& name);
    virtual ~Scene() override;

    bool IsActive() const;
    void SetActive(bool value);
    void Attach(GameObject* object);
    GameObject* Find(const std::string& name) const;
    GameObject* FindWithTag(const std::string& tag) const;
    std::vector<GameObject*> FindGameObjectsWithTag(const std::string& tag) const;

    // Only at once being called when scene object is created
    virtual void OnAwake() {}
    // being called whenever scene is activated
    virtual void OnStart() {}
    // Fixed time step update
    virtual void OnFixedUpdate() {}
    // every frame
    virtual void OnUpdate() {}
    // last update every frame after OnUpdate called
    virtual void OnLateUpdate() {}
    // Pre Rendering function
    virtual void OnPreRender() {}
    // Pre Rendering function
    virtual void OnAfterRender() {}
    // before being called SetActive function (false -> true)
    virtual void OnEnable() {}
    // before being called SetActive function (true -> false)
    virtual void OnDisable() {}
    // when scene object is destroyed
    virtual void OnDestroy() {}

  private:
    virtual void Update(float deltaTime) final;
    virtual void Render() final;
    virtual bool CompareEquality(const Object& rhs) const final;
    virtual bool Destroy() final;
    bool Remove(GameObject* object);

  protected:
    std::vector<GameObject*> gameObjects;
    std::vector<Light*> lights;
    std::vector<Camera*> cameras;
    std::vector<Collider*> colliders;
    int32_t buildIndex;
    SceneManager* sceneManager;
    bool isActive;
  };
}  // namespace Theodore

#endif /* Scene_h */
