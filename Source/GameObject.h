#ifndef GameObject_h
#define GameObject_h

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include "Component.h"
#include "Object.h"
#include "Enumeration.h"
#include "Light.h"
#include "Camera.h"
#include "Collider.h"

namespace Quark {
	class Transform; class Scene; class Debug;
	class GameObject : public Object {
		friend class Scene;
		friend class Debug;
		friend class Transform;
		friend class MeshRenderer;
	public:
		explicit GameObject(const std::string& name, Scene* scene);
		GameObject(const std::string& name, GameObject* parent, Scene* scene);
		virtual ~GameObject();

		template<typename T, typename ...Ts>
		T* AddComponent(Ts ... args);
		template<typename T>
		T* GetComponent();
		template<typename T>
		T* GetComponentInChildren();
		template<typename T>
		T* GetComponentInParent();

		bool IsActive() const;
		void SetActive(bool value);
		void SetActiveRecursive(bool value);

		void SetTag(const std::string& newTag);
		const std::string& GetTag() const;
		bool CompareTag(const std::string& tag) const;

		template<typename T>
		bool RemoveComponent();
		template<typename T>
		bool SendMessage(Message& msg);
		bool SendMessageUpwards(Message& msg);
		bool BroadcastMessage(Message& msg);
		static GameObject* CreatePrimitive(PrimitiveType type, Scene* scene);
		Transform* GetTransform() const;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

		template<typename T>
		bool SubscribeToMessageType(MessageType msgType);
		template<typename T>
		bool SubscribeAllMessageType();

		std::vector<Light*>& GetAllLights() const;
		std::vector<Camera*>& GetAllCameras() const;
		std::vector<Collider*> GetAllColliders() const;

		std::set<Component*> mSubscriber[TheNumberOfMessage];
		std::unordered_map<std::type_index, Component*> mComponents;
		std::vector<GameObject*> mChildren;
		GameObject* mParent;
		Scene* mScene;
		bool mActiveSelf;
		uint32_t mTag;
		std::string mTagString;
		Transform* mTransform;
	};

	// public member functions
	template<typename T, typename ... Ts>
	T* GameObject::AddComponent(Ts ... args) {
		// check if there is already exist.
		if (mComponents.find(std::type_index(typeid(T))) != mComponents.end())
			return static_cast<T*>(nullptr);

		// allocate with malloc() because of mGameObject member.
		T* component = static_cast<T*>(malloc(sizeof(T)));
		// check if component casting to T type were failed.
		if (!component)
			return static_cast<T*>(nullptr);
		// set the member variables...
		component->mGameObject = this;
		// call constructor.
		new (component) T(args...);
		// store to unordered_map(hash map).
		mComponents.insert(std::make_pair(std::type_index(typeid(T)), component));

		// caching light components
		Light* light = this->GetComponent<Light>();
		if (light && light->type != LightType::DirectionalLight) {
			mScene->mLights.push_back(light);
		}

		// caching camera components
		Camera* camera = this->GetComponent<Camera>();
		if (camera) {
			mScene->mCameras.push_back(camera);
		}

		// caching collider components
		//Collider* collider = this->GetComponent<Collider>();
		//if (collider) {
		//	mScene->mCollider.push_back(collider);
		//}

		return component;
	}

	template<typename T>
	T* GameObject::GetComponent() {
		// find STL algorithm with hash value
		auto iter = mComponents.find(std::type_index(typeid(T)));
		// not exists in component map.
		if (iter == mComponents.end())
			return static_cast<T*>(nullptr);

		// find and return it.
		return static_cast<T*>(iter->second);
	}

	template<typename T>
	T* GameObject::GetComponentInChildren() {
		// find matching T type component in children.
		// return first matching one.
		for (auto& i : mChildren) {
			T* value = i->GetComponent<T>();
			if (value)
				return value;
		}

		return static_cast<T*>(nullptr);
	}

	template<typename T>
	T* GameObject::GetComponentInParent() {
		// copy parent pointer.
		GameObject* iter = mParent;

		// loop while end of parent.
		while (iter != nullptr) {
			T* value = (*iter).GetComponent<T>();
			if (value) // if finds
				return value;
			iter = iter->mParent; // linking node with next.
		}

		return static_cast<T*>(nullptr);
	}

	template<typename T>
	bool GameObject::RemoveComponent() {
		auto iter = mComponents.find(std::type_index(typeid(T)));
		// not exists in cache.
		if (iter == mComponents.end())
			return false;

		if (iter->second) {
			iter->second->~Component();
			free(iter->second);
			iter->second = nullptr;
		}
		mComponents.erase(iter);
		return true;
	}

	template<typename T>
	bool GameObject::SendMessage(Message& msg) {
		auto ret = GetComponent<T>();
		if (!ret)
			return false;

		unsigned int base = msg.GetType();
		for (auto i : mSubscriber[base]) {
			if (i == ret)
				i->HandleMessage(msg);
		}

		return true;
	}

	template<typename T>
	bool GameObject::SubscribeToMessageType(MessageType msgType) {
		auto ret = GetComponent<T>();
		if (!ret)
			return false;

		mSubscriber[msgType].insert(ret);

		return true;
	}

	template<typename T>
	bool GameObject::SubscribeAllMessageType() {
		auto ret = GetComponent<T>();
		if (!ret)
			return false;

		for (int i = 0; i < TheNumberOfMessage; i++)
			mSubscriber[i].insert(ret);

		return true;
	}
}

#endif /* GameObject_h */