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

		GameObject* cube, *plane, *sphere, *cylinder, *torus, *cone;
		GameObject* cam2, *skybox;
		Light* light1;
		Light* light2;
		Light* light3;
		Light* light4;
		Light* light5;
		Light* light6;

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
		bool isWire;
		bool camChange;
		bool skyChange;
	};
}

#endif /* SplashScene_h */