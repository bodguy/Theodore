// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "GameObject.h"

#include "Asset/AssetManager.h"
#include "Asset/Shader.h"
#include "Asset/Texture.h"
#include "Asset/Texture2D.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "Component/Material.h"
#include "Asset/Mesh.h"
#include "Helper/ShapeGenerator.h"
#include "Helper/Utility.h"
#include "Helper/crc32.h"
#include "Scene.h"

namespace Theodore {
  GameObject::GameObject(const std::string& name, Scene* scene) : Object(name), parent(nullptr), scene(nullptr), activeSelf(true), tagName("untagged"), tag(0), transform(nullptr) {
    children.clear();
		transform = AddComponent<Transform>();
    scene->Attach(this);
  }

  GameObject::GameObject(const std::string& name, GameObject* parent, Scene* scene)
      : Object(name), parent(parent), scene(nullptr), activeSelf(true), tagName("untagged"), tag(0), transform(nullptr) {
    parent->children.push_back(this);
		transform = AddComponent<Transform>();
    scene->Attach(this);
  }

  GameObject::GameObject(const GameObject& other) : Object(other.name) {
    // TODO
  }

  GameObject::~GameObject() {
    for (auto& i : components) {
      if (i.second) {
        i.second->~Component();
        free(i.second);
        i.second = nullptr;
      }
    }
    components.clear();
    children.clear();
  }

  bool GameObject::IsActive() const { return activeSelf; }

  void GameObject::SetActive(bool value) { activeSelf = value; }

  void GameObject::SetActiveRecursive(bool value) {
    SetActive(value);
    for (auto& i : children) i->SetActiveRecursive(value);
  }

  void GameObject::SetTag(const std::string& newTag) {
		tagName = newTag;
		tag = CRC32_STR(newTag.c_str());
    // TagManager::AddTag(mTag);
  }

  const std::string& GameObject::GetTag() const { return tagName; }

  bool GameObject::CompareTag(const std::string& tag) const {
    if (tag == CRC32_STR(tag.c_str())) return true;

    return false;
  }

  //  GameObject GameObject::FindWithTag(const std::string& tag) {
  //    return new GameObject();
  //  }

  bool GameObject::SendMessageUpwards(Message& msg) {
    unsigned int base = msg.GetType();
    for (auto& i : subscriber[base]) i->HandleMessage(msg);

    GameObject* parent = parent;
    while (parent != nullptr) {
      for (auto& i : parent->subscriber[base]) i->HandleMessage(msg);
      parent = parent->parent;
    }

    return true;
  }

  bool GameObject::BroadcastMessage(Message& msg) {
    unsigned int base = msg.GetType();
    for (auto& i : subscriber[base]) i->HandleMessage(msg);

    if (!children.empty()) {
      for (auto& i : children) {
        for (auto& j : i->subscriber[base]) j->HandleMessage(msg);
      }
    }

    return true;
  }

  GameObject* GameObject::CreatePrimitive(PrimitiveType type, Scene* scene) {
    static std::string primitiveName[] = {"Plane", "Cube", "Sphere", "UVSphere", "Capsule", "Cylinder", "Torus", "Cone", "UtahTeapot", "Knots"};
    GameObject* primitive = new GameObject(primitiveName[static_cast<int>(type)], scene);
    Mesh* mesh = new Mesh();
    Material* material = new Material(Shader::Find("Phong"));
    MeshRenderer* rend = primitive->AddComponent<MeshRenderer>();

    switch (type) {
      case PrimitiveType::Plane:
        mesh = ShapeGenerator::GeneratePlane();
        break;
      case PrimitiveType::Cube:
        mesh = ShapeGenerator::GenerateCube();
        break;
      case PrimitiveType::Sphere:
        mesh = ShapeGenerator::GenerateIcoSphere();
        break;
      case PrimitiveType::UVSphere:  // Not Yet
        mesh = ShapeGenerator::GenerateUVSphere();
        break;
      case PrimitiveType::Capsule:  // Not Yet
        mesh = ShapeGenerator::GenerateCapsule();
        break;
      case PrimitiveType::Cylinder:  // Not Yet, cap is not work
        mesh = ShapeGenerator::GenerateCylinder();
        rend->SetPrimitive(Primitive::TriangleStrip);
        break;
      case PrimitiveType::Torus:
        mesh = ShapeGenerator::GenerateTorus();
        rend->SetPrimitive(Primitive::TriangleStrip);
        break;
      case PrimitiveType::Cone:
        mesh = ShapeGenerator::GenerateCone();
        rend->SetPrimitive(Primitive::TriangleFan);
        break;
      case PrimitiveType::UtahTeapot:  // Not Yet
        mesh = ShapeGenerator::GenerateUtahTeapot();
        break;
      case PrimitiveType::Knots:  // Not Yet
        mesh = ShapeGenerator::GenerateKnots();
        break;
    }

    rend->SetMaterial(material);
    rend->SetMesh(mesh);

    return primitive;
  }

  Transform* GameObject::GetTransform() const { return transform; }

  void GameObject::Update(float deltaTime) {
    for (auto& i : components) {
      if (i.second->IsEnabled()) {
        i.second->Update(deltaTime);
      }
    }
  }

  void GameObject::Render() {
    for (auto& i : components) {
      if (i.second->IsEnabled()) {
        i.second->Render();
      }
    }
  }

  bool GameObject::CompareEquality(const Object& rhs) const {
    const GameObject* t = dynamic_cast<const GameObject*>(&rhs);

    // compareing each mParent is not allowed.
    return !(!t || activeSelf != t->activeSelf || tag != t->tag || !Utility::CompareUnorderedmap(components, t->components) || !Utility::CompareVector(children, t->children));
  }

  bool GameObject::Destroy() {
    for (auto& i : children) {
      if (scene) scene->Remove(i);
    }

    if (scene) return scene->Remove(this);

    return false;
  }

  std::vector<Light*>& GameObject::GetAllLights() const { return scene->lights; }

  std::vector<Camera*>& GameObject::GetAllCameras() const { return scene->cameras; }

  std::vector<Collider*> GameObject::GetAllColliders() const { return scene->colliders; }
}  // namespace Theodore
