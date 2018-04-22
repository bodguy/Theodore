#ifndef Application_h
#define Application_h

#include <string>
#include "Enumeration.h"

namespace Quark {
	class Platform; class Input; class Time; class SceneManager; class AssetManager;
	class Application {
	public:
		Application();
		~Application();

		bool Initialize(const std::string& name, int width, int height, bool fullmode = false, int majorVersion = 4, int minorVersion = 3, int multisample = 16, Enumeration::WindowStyle style = Enumeration::Resizable);
		void Run();

	private:
		static Application* instance;

		void Render();
		void Update(double deltaTime);

		Platform* mPlatform;
		Input* mInput;
		Time* mTime;
		SceneManager* mSceneManager;
		AssetManager* mAssetManager;
	};
}

#endif /* Application_h */