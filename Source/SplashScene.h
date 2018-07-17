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
		void CameraUpdate();

		GameObject *plane, *monkey, *cube;
		Mesh* monkeyMesh, *cubeMesh;
		Plane* planes;
		Transform* camTrans;
		Vector3d boxPos, camPos;
		Ray ray;
		BoxCollider* boxCollider;
		SphereCollider* sphereCollider;

		float speed;
		float rotationY;
		float rotationX;
		float sensitivity;
		float moveSensitivity;
	};
}

#endif /* SplashScene_h */