#include "GameObject.h"
#include "Scene.h"
#include "crc32.h"
#include "Utility.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "PrimitiveData.h"

namespace Quark {
	GameObject::GameObject(const std::string& name, Scene* scene)
		: Object(name), mParent(nullptr), mScene(nullptr), mActiveSelf(true), mTagString("untagged"), mTag(0), mTransform(nullptr) {
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

	bool GameObject::IsActive() const {
		return mActiveSelf;
	}

	void GameObject::SetActive(bool value) {
		mActiveSelf = value;
	}

	void GameObject::SetActiveRecursive(bool value) {
		SetActive(value);
		for (auto& i : mChildren)
			i->SetActiveRecursive(value);
	}

	void GameObject::SetTag(const std::string& newTag) {
		mTagString = newTag;
		mTag = CRC32_STR(newTag.c_str());
		//TagManager::AddTag(mTag);
	}

	const std::string& GameObject::GetTag() const {
		return mTagString;
	}

	bool GameObject::CompareTag(const std::string& tag) const {
		if (mTag == CRC32_STR(tag.c_str()))
			return true;

		return false;
	}

	bool GameObject::SendMessageUpwards(Message& msg) {
		unsigned int base = msg.GetType();
		for (auto& i : mSubscriber[base])
			i->HandleMessage(msg);

		GameObject* parent = mParent;
		while (parent != nullptr) {
			for (auto& i : parent->mSubscriber[base])
				i->HandleMessage(msg);
			parent = parent->mParent;
		}

		return true;
	}

	bool GameObject::BroadcastMessage(Message& msg) {
		unsigned int base = msg.GetType();
		for (auto& i : mSubscriber[base])
			i->HandleMessage(msg);

		if (!mChildren.empty()) {
			for (auto& i : mChildren) {
				for (auto& j : i->mSubscriber[base])
					j->HandleMessage(msg);
			}
		}
		 
		return true;
	}

	GameObject* GameObject::CreatePrimitive(Enumeration::PrimitiveType type, Scene* scene) {
		static std::string primitiveName[] = { "Plane", "Cube", "Sphere", "Capsule", "Cylinder" };
		GameObject* primitive = new GameObject(primitiveName[static_cast<int>(type)], scene);
		Mesh* mesh = new Mesh();
		Material* material = new Material();

		switch (type) {
		case Enumeration::PrimitiveType::Plane:
		{
			//float planeVertices[300] = { 0, };
			//for (int x = 0; x < 10; x++) {
			//	for (int y = 0; y < 10; y++) {
			//		planeVertices[y + x * 10 + 0] = -0.5f + (0.1f * y); // x
			//		planeVertices[y + x * 10 + 1] = 0.f; // y
			//		planeVertices[y + x * 10 + 2] = -0.5f + (0.1f * y); // z
			//	}
			//}
			//mesh->SetVertices(planeVertices);
			break;
		}
		case Enumeration::PrimitiveType::Cube:
			mesh->SetVertices(cubeVertices);
			break;
		case Enumeration::PrimitiveType::Sphere:
			break;
		case Enumeration::PrimitiveType::Capsule:
			break;
		case Enumeration::PrimitiveType::Cylinder:
			break;
		}

		primitive->AddComponent<MeshFilter>()->SetMesh(mesh);
		primitive->AddComponent<MeshRenderer>()->SetMaterial(material);
		return primitive;
	}

	Transform* GameObject::GetTransform() const {
		return mTransform;
	}

	void GameObject::Update(double deltaTime) {
		for (auto& i : mComponents)
			i.second->Update(deltaTime);
	}

	void GameObject::Render() {
		for (auto& i : mComponents)
			i.second->Render();
	}

	bool GameObject::CompareEquality(const Object& rhs) const {
		const GameObject* t = dynamic_cast<const GameObject*>(&rhs);

		// compareing each mParent is not allowed.
		if (!t || mActiveSelf != t->mActiveSelf || mTag != t->mTag ||
			!Utility::CompareUnorderedmap(mComponents, t->mComponents) ||
			!Utility::CompareVector(mChildren, t->mChildren))
			return false;

		return true;
	}

	bool GameObject::Destroy() {
		for (auto& i : mComponents) {
			if (i.second) {
				i.second->~Component();
				free(i.second);
				i.second = nullptr;
			}
		}
		mComponents.clear();
		mChildren.clear();

		if (mScene)
			return mScene->Remove(this);

		return true;
	}	
}
