#include "SplashScene.h"

namespace Theodore {
	SplashScene::SplashScene() : Scene("SplashScene") {
		speed = 20.f;
		rotationY = 0.f;
		rotationX = 0.f;
		sensitivity = 8.0f;
		moveSensitivity = 0.5f;
		fieldOfView = 60.f;
	}

	SplashScene::~SplashScene() {

	}

	void SplashScene::OnAwake() {
		GameObject* gizmo = new GameObject("gizmo", this);
		gizmo->AddComponent<Gizmo>(GizmoType::Translation);

		plane = GameObject::CreatePrimitive(PrimitiveType::Plane, this);
		plane->GetTransform()->SetLocalPosition(Vector3d(0.f, -1.f, 0.f));
		plane->AddComponent<BoxCollider>();
		plane->AddComponent<SphereCollider>();

		monkey = new GameObject("monkey", this);
		rend = monkey->AddComponent<MeshRenderer>();
		Material* mat = new Material(Shader::Find("Phong"));
		rend->SetMaterial(mat);
		rend->SetMesh(AssetManager::RequestMesh("Contents/model/dragon.obj"));
		monkey->AddComponent<BoxCollider>();
		monkey->AddComponent<SphereCollider>();

		cube = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
		cube->AddComponent<BoxCollider>();
		cube->AddComponent<SphereCollider>();
		Transform* cubeTrans = cube->GetComponent<Transform>();
		cubeTrans->SetParent(monkey);
		cubeTrans->SetLocalPosition(Vector3d(10.f, 0.f, 0.f));

		GameObject* pointLight = new GameObject("pointLight", this);
		Light* pl = pointLight->AddComponent<Light>(LightType::PointLight);
		pl->ambient = Color::white;
		pl->diffuse = Color::white;
		pl->specular = Color::white;
		pl->GetTransform()->SetLocalPosition(Vector3d::zero);

		SceneManager::GetMainCamera()->GetTransform()->SetLocalPosition(Vector3d(0.f, 15.f, 30.f));
		SceneManager::GetMainCamera()->GetTransform()->Rotate(Vector3d::right, -40.f);

		camTrans = SceneManager::GetMainCamera()->GetTransform();
	}

	void SplashScene::OnUpdate() {
		// ray casting
		if (Input::GetMouseButtonDown(MOUSE_LEFT)) {
			camPos = camTrans->GetPosition();
			ray = SceneManager::GetMainCamera()->ScreenPointToRay(Input::GetMousePosition());
		}

		if(rend->GetBounds()->IntersectRay(ray)) {
			Debug::Log("Ray and bounds intersects!");
		}
		
		Graphics::DrawLine(Vector3d(camPos.x, camPos.y, camPos.z + SceneManager::GetMainCamera()->GetNearClipPlane()
		), ray.GetPoint(100.f), Color::yellow);

		ObjectUpdate();
		CameraUpdate();
	}

	void SplashScene::ObjectUpdate() {
		monkey->GetTransform()->Rotate(Vector3d(1.f, 1.f, 1.f), Time::DeltaTime() * 20.f);

		if (Input::GetKeyHeld(KEY_LEFT)) {
			monkey->GetTransform()->Translate(Vector3d::left * Time::DeltaTime() * 10.f);
		}
		else if (Input::GetKeyHeld(KEY_RIGHT)) {
			monkey->GetTransform()->Translate(Vector3d::right * Time::DeltaTime() * 10.f);
		}
		else if (Input::GetKeyHeld(KEY_UP)) {
			monkey->GetTransform()->Translate(Vector3d::up * Time::DeltaTime() * 10.f);
		}
		else if (Input::GetKeyHeld(KEY_DOWN)) {
			monkey->GetTransform()->Translate(Vector3d::down * Time::DeltaTime() * 10.f);
		}

		if (Input::GetKeyHeld(KEY_RETURN)) {
			monkey->GetTransform()->Scale(Vector3d(0.2f, 0.2f, 0.2f));
		}
		else if (Input::GetKeyHeld(KEY_BACKSPACE)) {
			monkey->GetTransform()->Scale(Vector3d(-0.2f, -0.2f, -0.2f));
		}
	}

	void SplashScene::CameraUpdate() {
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}

		Camera* cam = SceneManager::GetMainCamera();
		Transform* trans = cam->GetTransform();
		Graphics::DrawFrustum(trans->GetLocalPosition(), cam->GetFieldOfView(), cam->GetFarClipPlane() / 500.f, cam->GetNearClipPlane(), cam->GetAspectRatio(), Color::white);

		//if (Input::GetMouseButtonHeld(MOUSE_LEFT)) {
		//	Vector3d right = SceneManager::GetMainCamera()->GetTransform()->GetRight();
		//	right *= Input::GetMouseDeltaPosition().x * moveSensitivity * Time::DeltaTime();
		//	SceneManager::GetMainCamera()->GetTransform()->Translate(right);

		//	Vector3d up = SceneManager::GetMainCamera()->GetTransform()->GetUp();
		//	up *= Input::GetMouseDeltaPosition().y * moveSensitivity * Time::DeltaTime();
		//	SceneManager::GetMainCamera()->GetTransform()->Translate(-up);
		//}

		if (Input::GetKeyHeld(KEY_LSHIFT) || Input::GetKeyHeld(KEY_RSHIFT)) {
			speed = 100.f;
		}
		else {
			speed = 20.f;
		}

		// camera translation
		if (Input::GetKeyHeld(KEY_D)) {
			Vector3d right = trans->GetRight();
			right *= speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(right);
		}
		else if (Input::GetKeyHeld(KEY_A)) {
			Vector3d left = trans->GetRight();
			left *= -speed * Time::DeltaTime();
			trans->Translate(left);
		}
		else if (Input::GetKeyHeld(KEY_W)) {
			Vector3d forward = trans->GetForward();
			forward *= speed * Time::DeltaTime();
			trans->Translate(forward);
		}
		else if (Input::GetKeyHeld(KEY_S)) {
			Vector3d backward = trans->GetForward();
			backward *= -speed * Time::DeltaTime();
			trans->Translate(backward);
		}
		else if (Input::GetKeyHeld(KEY_Q)) {
			Vector3d down = trans->GetUp();
			down *= -speed * Time::DeltaTime();
			trans->Translate(down);
		}
		else if (Input::GetKeyHeld(KEY_E)) {
			Vector3d up = trans->GetUp();
			up *= speed * Time::DeltaTime();
			trans->Translate(up);
		}

		// camera rotation
		if (Input::GetMouseButtonHeld(MOUSE_RIGHT)) {
			// zoom in, out
			if (Input::GetKeyHeld(KEY_V)) {
				fieldOfView++;
			} else if (Input::GetKeyHeld(KEY_C)) {
				fieldOfView--;
			}
			cam->SetFieldOfView(fieldOfView);

			rotationY = Input::GetMouseDeltaPosition().x * sensitivity * Time::DeltaTime();
			rotationX = Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();

			trans->Rotate(Vector3d::up, -rotationY);
			trans->Rotate(trans->GetRight(), -rotationX);
		} else {
			fieldOfView = 60.f;
			cam->ResetFieldOfView();
		}
	}
}