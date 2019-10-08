// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "GameObject.h"
#include "Asset/AssetManager.h"
#include "Asset/Shader.h"
#include "Asset/Texture.h"
#include "Asset/Texture2D.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "Component/sub/Material.h"
#include "Component/sub/Mesh.h"
#include "Component/sub/ShapeGenerator.h"
#include "Helper/Utility.h"
#include "Helper/crc32.h"
#include "Scene.h"

namespace Theodore {
  GameObject::GameObject(const std::string& name, Scene* scene) : Object(name), mParent(nullptr), mScene(nullptr), mActiveSelf(true), mTagString("untagged"), mTag(0), mTransform(nullptr) {
    mChildren.clear();
    mTransform = AddComponent<Transform>();
    scene->Attach(this);
  }

  GameObject::GameObject(const std::string& name, GameObject* parent, Scene* scene)
      : Object(name), mParent(parent), mScene(nullptr), mActiveSelf(true), mTagString("untagged"), mTag(0), mTransform(nullptr) {
    mParent->mChildren.push_back(this);
    mTransform = AddComponent<Transform>();
    scene->Attach(this);
  }

  GameObject::GameObject(const GameObject& other) : Object(other.mName) {
    // TODO
  }

  GameObject::~GameObject() {
    for (auto& i : mComponents) {
      if (i.second) {
        i.second->~Component();
        free(i.second);
        i.second = nullptr;
      }
    }
    mComponents.clear();
    mChildren.clear();
  }

  bool GameObject::IsActive() const { return mActiveSelf; }

  void GameObject::SetActive(bool value) { mActiveSelf = value; }

  void GameObject::SetActiveRecursive(bool value) {
    SetActive(value);
    for (auto& i : mChildren) i->SetActiveRecursive(value);
  }

  void GameObject::SetTag(const std::string& newTag) {
    mTagString = newTag;
    mTag = CRC32_STR(newTag.c_str());
    // TagManager::AddTag(mTag);
  }

  const std::string& GameObject::GetTag() const { return mTagString; }

  bool GameObject::CompareTag(const std::string& tag) const {
    if (mTag == CRC32_STR(tag.c_str())) return true;

    return false;
  }

  //  GameObject GameObject::FindWithTag(const std::string& tag) {
  //    return new GameObject();
  //  }

  bool GameObject::SendMessageUpwards(Message& msg) {
    unsigned int base = msg.GetType();
    for (auto& i : mSubscriber[base]) i->HandleMessage(msg);

    GameObject* parent = mParent;
    while (parent != nullptr) {
      for (auto& i : parent->mSubscriber[base]) i->HandleMessage(msg);
      parent = parent->mParent;
    }

    return true;
  }

  bool GameObject::BroadcastMessage(Message& msg) {
    unsigned int base = msg.GetType();
    for (auto& i : mSubscriber[base]) i->HandleMessage(msg);

    if (!mChildren.empty()) {
      for (auto& i : mChildren) {
        for (auto& j : i->mSubscriber[base]) j->HandleMessage(msg);
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

  Transform* GameObject::GetTransform() const { return mTransform; }

  void GameObject::Update(float deltaTime) {
    for (auto& i : mComponents) {
      if (i.second->IsEnabled()) {
        i.second->Update(deltaTime);
      }
    }
  }

  void GameObject::Render() {
    for (auto& i : mComponents) {
      if (i.second->IsEnabled()) {
        i.second->Render();
      }
    }
  }

  bool GameObject::CompareEquality(const Object& rhs) const {
    const GameObject* t = dynamic_cast<const GameObject*>(&rhs);

    // compareing each mParent is not allowed.
    return !(!t || mActiveSelf != t->mActiveSelf || mTag != t->mTag ||
             !Utility::CompareUnorderedmap(mComponents, t->mComponents) ||
             !Utility::CompareVector(mChildren, t->mChildren));
  }

  bool GameObject::Destroy() {
    for (auto& i : mChildren) {
      if (mScene) mScene->Remove(i);
    }

    if (mScene) return mScene->Remove(this);

    return false;
  }

  std::vector<Light*>& GameObject::GetAllLights() const { return mScene->mLights; }

  std::vector<Camera*>& GameObject::GetAllCameras() const { return mScene->mCameras; }

  std::vector<Collider*> GameObject::GetAllColliders() const { return mScene->mCollider; }
}  // namespace Theodore
