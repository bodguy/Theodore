#include "GameObject.h"
#include "Scene.h"
#include "crc32.h"
#include "Utility.h"
#include "Transform.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"

namespace Quark {
	GameObject::GameObject(const std::string& name, Scene* scene)
		: Object(name), mParent(nullptr), mScene(nullptr), mActiveSelf(true), mTagString("untagged"), mTag(0), mTransform(nullptr) {
		mChildren.clear();
		mTransform = AddComponent<Transform>();
		scene->Attach(this);
	}

	GameObject::GameObject(const std::string& name, Scene* scene, GameObject* parent)
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

	GameObject* GameObject::GetParent() const {
		return mParent;
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
		const float Verts[] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		const float Normals[] = {
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,
			0.0f,  0.0f, -1.0f,

			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,
			0.0f,  0.0f,  1.0f,

			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,

			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,
			1.0f,  0.0f,  0.0f,

			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,

			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f,
			0.0f,  1.0f,  0.0f
		};

		GameObject* temp = new GameObject("Cube", scene);
		Mesh* mesh = new Mesh();
		mesh->SetVertices(Verts);
		mesh->SetNormals(Normals);
		temp->AddComponent<MeshFilter>()->SetMesh(mesh);

		Material* material = new Material();
		temp->AddComponent<MeshRenderer>()->SetMaterial(material);
		return temp;
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
