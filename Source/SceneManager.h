#ifndef SceneManager_h
#define SceneManager_h

#include <vector>
#include "Scene.h"

namespace Quark {
	class Camera; class Scene; class Light;
	class SceneManager {
		friend class Scene;
	public:
		SceneManager();
		~SceneManager();

		// Get SceneManager static instance.
		static SceneManager* GetInstance();
		// Create new scene and increment sceneIndex.
		template<typename T, typename ...Ts>
		static Scene* CreateScene(const std::string& sceneName, Ts ... args);
		// Get current active scene instance.
		static Scene* GetActiveScene();
		// Set current active scene.
		static bool SetActiveScene(Scene* scene);
		// Get scene at scene count.
		static Scene* GetSceneAt(uint32_t sceneBuildIndex);
		// Get scene by its name.
		static Scene* GetSceneByName(const std::string& sceneName);
		// Load scene from index and set current scene.
		static bool LoadScene(uint32_t sceneBuildIndex);
		// Load scene from scene anme and set current scene.
		static bool LoadScene(const std::string& sceneName);
		// Get total scene count.
		static uint32_t GetSceneCount();
		// Get Main Camera that currently active scene
		static Camera* GetMainCamera();
		// Set Main Camera
		static void SetMainCamera(Camera* cam);
		// Reset to the Main Camera
		static void SetCurrentCamera();
		// Get Global Light aht currently active scene
		static Light* GetGlobalLight();
		// Set Global light
		static void SetGlobalLight(Light* light);
		// Reset to the global light
		static void SetCurrentLigh();

		// Update current active scene in main game loop.
		void Update(double deltaTime) const;
		// Render current active scene in main game loop.
		void Render() const;

	private:
		bool Remove(const std::string& sceneName);

		std::vector<Scene*> mScenes;
		Scene* mCurrentScene;
		uint32_t mSceneCount;
		Camera* mMainCamera;
		Light* mGlobalLight;
		static SceneManager* instance;
	};

	template<typename T, typename ...Ts>
	Scene* SceneManager::CreateScene(const std::string& sceneName, Ts ... args) {
		// find already exist scene in mScenes.
		if (GetSceneByName(sceneName))
			return nullptr;

		// if there is not scene named sceneName here, and then build scene.
		// Notice that scene has to initialized with malloc because of the SceneManager member that scene class has been.
		// reminder, cause scene instance is allocated with malloc(), the instance of scene has to be dealloc with free()
		// and deinitialized with calling destructor manually.
		Scene* scene = static_cast<T*>(malloc(sizeof(T)));
		scene->mManager = GetInstance();
		// assign the scene count that it will be stored.
		scene->mBuildIndex = scene->mManager->mSceneCount;
		// then, call c++ constructor with placement new operator.
		new (scene) T(args...); // parameterize variadic constructor members.
		scene->OnAwake(); // call awake function before stored it
		GetInstance()->mScenes.push_back(scene); // store new build scene in array.
		GetInstance()->mSceneCount++; // increment scene counter.
		SetCurrentCamera(); // restore to current scene main camera as the scena manager main camera.
		SetCurrentLigh(); // retore to current scene global light source.

		// succesfully initialized the scene then return it.
		return scene;
	}
}
#endif /* SceneManager_h */
