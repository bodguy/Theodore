/**
	@brief intro scene
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef SplashScene_h
#define SplashScene_h

#include "TheodoreEngine.h"

namespace Theodore {
	class SplashScene : public Scene {
	public:
		SplashScene();
		virtual ~SplashScene();

		virtual void OnAwake() override;
		virtual void OnUpdate() override;

	private:
		void ObjectUpdate();
		void CameraUpdate();

		GameObject *plane, *monkey;
		Transform* camTrans;
		Vector3d camPos;
		Ray ray;

		float speed;
		float rotationY;
		float rotationX;
		float sensitivity;
		float moveSensitivity;
	};
}

#endif /* SplashScene_h */