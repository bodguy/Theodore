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
		void CameraUpdate();

		GameObject *plane, *monkey, *cube;
		Plane* planes;

		Vector3d camPos, hitPoint;

		float speed;
		float rotationY;
		float rotationX;
		float sensitivity;
		float moveSensitivity;
	};
}

#endif /* SplashScene_h */