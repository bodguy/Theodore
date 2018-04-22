#include "SceneManager.h"
#include "crc32.h"
#include "SplashScene.h"

namespace Quark {
	SceneManager* SceneManager::instance = nullptr;
	SceneManager::SceneManager() :mCurrentScene(nullptr), mSceneCount(0) {
		instance = this;
		mScenes.clear();
		GetInstance()->SetActiveScene(CreateScene<SplashScene>("SplashScene"));
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

	SceneManager* SceneManager::GetInstance() {
		return instance;
	}

	Scene* SceneManager::GetActiveScene() {
		return GetInstance()->mCurrentScene;
	}

	bool SceneManager::SetActiveScene(Scene* scene) {
		if (!scene)
			return false;

		GetInstance()->mCurrentScene = scene;
		scene->OnStart();

		return true;
	}

	Scene* SceneManager::GetSceneAt(uint32_t sceneBuildIndex) {
		return GetInstance()->mScenes.at(sceneBuildIndex);
	}

	Scene* SceneManager::GetSceneByName(const std::string& sceneName) {
		uint32_t find = CRC32_STR(sceneName.c_str());
		for (auto& i : GetInstance()->mScenes) {
			if (i->GetHashCode() == find)
				return i;
		}

		return nullptr;
	}

	bool SceneManager::LoadScene(uint32_t sceneBuildIndex) {
		return SetActiveScene(GetSceneAt(sceneBuildIndex));
	}

	bool SceneManager::LoadScene(const std::string& sceneName) {
		return SetActiveScene(GetSceneByName(sceneName));
	}

	uint32_t SceneManager::GetSceneCount() {
		return GetInstance()->mSceneCount;
	}

	void SceneManager::Update(double deltaTime) const {
		if (mCurrentScene && mCurrentScene->mActive)
			mCurrentScene->Update(deltaTime);
	}

	void SceneManager::Render() const {
		if (mCurrentScene && mCurrentScene->mActive)
			mCurrentScene->Render();
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
}