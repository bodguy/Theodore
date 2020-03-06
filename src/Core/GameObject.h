// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef GameObject_h
#define GameObject_h

#include <set>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "Component/Camera.h"
#include "Component/Collider/BoxCollider.h"
#include "Component/Collider/Collider.h"
#include "Component/Collider/SphereCollider.h"
#include "Component/Component.h"
#include "Component/Light.h"
#include "Graphics/Enumeration.h"
#include "Object.h"
#include "Scene.h"

namespace Theodore {
  class Transform;
  class Debug;
  class GameObject : public Object {
    friend class Scene;
    friend class Debug;
    friend class Transform;
    friend class MeshRenderer;
    friend class Object;

  public:
    explicit GameObject(const std::string& name, Scene* scene);
    GameObject(const std::string& name, GameObject* parent, Scene* scene);
    GameObject(const GameObject& other);  // copy constructor
    virtual ~GameObject() override;

    template <typename T, typename... Ts>
    T* AddComponent(Ts... args);
    template <typename T>
    T* GetComponent();
    template <typename T>
    T* GetComponentInChildren();
    template <typename T>
    T* GetComponentInParent();

    bool IsActive() const;
    void SetActive(bool value);
    void SetActiveRecursive(bool value);

    void SetTag(const std::string& newTag);
    const std::string& GetTag() const;
    bool CompareTag(const std::string& tag) const;
    //    static GameObject FindWithTag(const std::string& tag);

    template <typename T>
    bool RemoveComponent();
    template <typename T>
    bool SendMessage(Message& msg);
    bool SendMessageUpwards(Message& msg);
    bool BroadcastMessage(Message& msg);
    static GameObject* CreatePrimitive(PrimitiveType type, Scene* scene);
    Transform* GetTransform() const;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

    template <typename T>
    bool SubscribeToMessageType(MessageType msgType);
    template <typename T>
    bool SubscribeAllMessageType();

    std::vector<Light*>& GetAllLights() const;
    std::vector<Camera*>& GetAllCameras() const;
    std::vector<Collider*> GetAllColliders() const;

    std::set<Component*> subscriber[TheNumberOfMessage];
    std::unordered_map<std::type_index, Component*> components;
    std::vector<GameObject*> children;
    GameObject* parent;
    Scene* scene;
    bool activeSelf;
    uint32_t tag;
    std::string tagName;
    Transform* transform;
  };

  // public member functions
  template <typename T, typename... Ts>
  T* GameObject::AddComponent(Ts... args) {
    // check if there is already exist.
    if (components.find(std::type_index(typeid(T))) != components.end()) return static_cast<T*>(nullptr);

    // allocate with malloc() because of mGameObject member.
    T* component = static_cast<T*>(malloc(sizeof(T)));
    // check if component casting to T type were failed.
    if (!component) return static_cast<T*>(nullptr);
    // set the member variables...
    component->mGameObject = this;
    // placement new and call constructor.
    new (component) T(args...);
    // store to unordered_map(hash map).
    components.insert(std::make_pair(std::type_index(typeid(T)), component));

    // caching light components
    Light* light = this->GetComponent<Light>();
    if (light && light->type != LightType::DirectionalLight) {
      scene->lights.push_back(light);
    }

    // caching camera components
    Camera* camera = this->GetComponent<Camera>();
    if (camera) {
      scene->cameras.push_back(camera);
    }

    // caching collider components
    Collider* collider = this->GetComponent<BoxCollider>();
    if (collider) {
      scene->colliders.push_back(collider);
    }
    collider = this->GetComponent<SphereCollider>();
    if (collider) {
      scene->colliders.push_back(collider);
    }

    return component;
  }

  template <typename T>
  T* GameObject::GetComponent() {
    // find STL algorithm with hash value
    auto iter = components.find(std::type_index(typeid(T)));
    // not exists in component map.
    if (iter == components.end()) return static_cast<T*>(nullptr);

    // find and return it.
    return static_cast<T*>(iter->second);
  }

  template <typename T>
  T* GameObject::GetComponentInChildren() {
    // find matching T type component in children.
    // return first matching one.
    for (auto& i : children) {
      T* value = i->GetComponent<T>();
      if (value) return value;
    }

    return static_cast<T*>(nullptr);
  }

  template <typename T>
  T* GameObject::GetComponentInParent() {
    // copy parent pointer.
    GameObject* iter = parent;

    // loop while end of parent.
    while (iter != nullptr) {
      T* value = (*iter).GetComponent<T>();
      if (value)  // if finds
        return value;
      iter = iter->parent;  // linking node with next.
    }

    return static_cast<T*>(nullptr);
  }

  template <typename T>
  bool GameObject::RemoveComponent() {
    auto iter = components.find(std::type_index(typeid(T)));
    // not exists in cache.
    if (iter == components.end()) return false;

    if (iter->second) {
      iter->second->~Component();
      free(iter->second);
      iter->second = nullptr;
    }
    components.erase(iter);
    return true;
  }

  template <typename T>
  bool GameObject::SendMessage(Message& msg) {
    auto ret = GetComponent<T>();
    if (!ret) return false;

    unsigned int base = msg.GetType();
    for (auto i : subscriber[base]) {
      if (i == ret) i->HandleMessage(msg);
    }

    return true;
  }

  template <typename T>
  bool GameObject::SubscribeToMessageType(MessageType msgType) {
    auto ret = GetComponent<T>();
    if (!ret) return false;

    subscriber[msgType].insert(ret);

    return true;
  }

  template <typename T>
  bool GameObject::SubscribeAllMessageType() {
    auto ret = GetComponent<T>();
    if (!ret) return false;

    for (int i = 0; i < TheNumberOfMessage; i++) subscriber[i].insert(ret);

    return true;
  }
}  // namespace Theodore

#endif /* GameObject_h */
