/**
  @brief scene container for scenemanager
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Scene_h
#define Scene_h

#include "Object.h"
#include <memory>
#include <vector>

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
    virtual ~Scene();

    bool IsActive() const;
    void SetActive(bool value);
    void Attach(GameObject* object);
    GameObject* Find(const std::string& name) const;
    GameObject* FindWithTag(const std::string& tag) const;
    std::shared_ptr<std::vector<GameObject*>> FindGameObjectsWithTag(const std::string& tag) const;

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
    std::vector<GameObject*> mGameObjects;
    std::vector<Light*> mLights;
    std::vector<Camera*> mCameras;
    std::vector<Collider*> mCollider;
    int32_t mBuildIndex;
    SceneManager* mManager;
    bool mActive;
  };
} // namespace Theodore

#endif /* Scene_h */
