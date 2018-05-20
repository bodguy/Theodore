#ifndef SplashScene_h
#define SplashScene_h

#include "QuarkEngine.h"

namespace Quark {
	class SplashScene : public Scene {
	public:
		SplashScene();
		virtual ~SplashScene();

		virtual void OnAwake() override;
		virtual void OnUpdate() override;

	private:
		void ObjectUpdate();
		void CameraUpdate();

		GameObject* cube, *plane, *sphere, *cylinder, *torus;

		float speed;
		float rotationY;
		float rotationX;
		float sensitivity;
		float moveSensitivity;

		float minimumX;
		float maximumX;

		float minimumY;
		float maximumY;
		bool isOrtho;
	};
}

#endif /* SplashScene_h */