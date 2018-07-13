#include "SplashScene.h"

namespace Theodore {
	SplashScene::SplashScene() : Scene("SplashScene"), boxPos(1.f, 5.5f, 0.f) {
		speed = 4.5f;
		rotationY = 0.f;
		rotationX = 0.f;
		sensitivity = 8.0f;
		moveSensitivity = 0.5f;
	}

	SplashScene::~SplashScene() {
		SafeDealloc(planes);
	}

	void SplashScene::OnAwake() {
		GameObject* gizmo = new GameObject("gizmo", this);
		gizmo->AddComponent<Gizmo>(GizmoType::Translation);

		plane = GameObject::CreatePrimitive(PrimitiveType::Plane, this);
		plane->GetTransform()->SetLocalPosition(Vector3d(0.f, -1.f, 0.f));

		monkey = new GameObject("monkey", this);
		MeshRenderer* rend = monkey->AddComponent<MeshRenderer>();
		Material* mat = new Material(Shader::Find("Phong"));
		rend->SetMaterial(mat);
		monkeyMesh = AssetManager::RequestMesh("Contents/model/dragon.obj");
		rend->SetMesh(monkeyMesh);
		rend->SetVisibleGizmos(true);
		boxCollider = monkey->AddComponent<BoxCollider>();
		boxCollider->SetVisible(true);
		sphereCollider = monkey->AddComponent<SphereCollider>();
		sphereCollider->SetVisible(true);

		cube = new GameObject("cube", monkey, this);
		cube->GetTransform()->SetLocalPosition(Vector3d(5.f, 0.f, 0.f));
		MeshRenderer* rend2 = cube->AddComponent<MeshRenderer>();
		Material* mat2 = new Material(Shader::Find("Phong"));
		mat2->texture0 = AssetManager::RequestTexture("Contents/BlueGrid.png", TextureFormat::RGBA32);
		cubeMesh = ShapeGenerator::GenerateCube();
		rend2->SetMaterial(mat2);
		rend2->SetMesh(cubeMesh);
		rend2->SetVisibleGizmos(true);
		BoxCollider* cubeBoxColl = cube->AddComponent<BoxCollider>();
		cubeBoxColl->SetVisible(true);

		GameObject* pointLight = new GameObject("pointLight", this);
		Light* pl = pointLight->AddComponent<Light>(LightType::PointLight);
		pl->ambient = Color::white;
		pl->diffuse = Color::white;
		pl->specular = Color::white;
		pl->GetTransform()->SetLocalPosition(Vector3d::zero);
		
		SceneManager::GetMainCamera()->GetTransform()->SetLocalPosition(Vector3d(0.f, 6.5f, 5.f));
		SceneManager::GetMainCamera()->GetTransform()->Rotate(Vector3d::right, -25.f);

		float distanceZ = -10.f;
		Vector3d distanceFromCamera = Vector3d(SceneManager::GetMainCamera()->GetTransform()->GetLocalPosition().x,
			SceneManager::GetMainCamera()->GetTransform()->GetLocalPosition().y, distanceZ);
		planes = new Plane(Vector3d::forward, distanceFromCamera);
		camTrans = SceneManager::GetMainCamera()->GetTransform();
	}

	void SplashScene::OnUpdate() {
		monkey->GetTransform()->Rotate(Vector3d::forward, Time::DeltaTime() * 40.f);
		cube->GetTransform()->Rotate(Vector3d::forward, Time::DeltaTime() * 120.f);

		// ray casting
		if (Input::GetMouseButtonDown(MOUSE_LEFT)) {
			camPos = camTrans->GetPosition();
			ray = SceneManager::GetMainCamera()->ScreenPointToRay(Input::GetMousePosition());
		}
		
		if (boxCollider->Raycast(ray, RaycastHit(), 100.f)) {
			Debug::Log("Ray Intersect");
		}
		Graphics::DrawLine(Vector3d(camPos.x, camPos.y, camPos.z + SceneManager::GetMainCamera()->GetNearClipPlane()
		), ray.GetPoint(100.f), Color::yellow);

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

		CameraUpdate();
	}

	void SplashScene::CameraUpdate() {
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}

		//if (Input::GetMouseButtonHeld(MOUSE_LEFT)) {
		//	Vector3d right = SceneManager::GetMainCamera()->GetTransform()->GetRight();
		//	right *= Input::GetMouseDeltaPosition().x * moveSensitivity * Time::DeltaTime();
		//	SceneManager::GetMainCamera()->GetTransform()->Translate(right);

		//	Vector3d up = SceneManager::GetMainCamera()->GetTransform()->GetUp();
		//	up *= Input::GetMouseDeltaPosition().y * moveSensitivity * Time::DeltaTime();
		//	SceneManager::GetMainCamera()->GetTransform()->Translate(-up);
		//}

		if (Input::GetKeyHeld(KEY_LSHIFT) || Input::GetKeyHeld(KEY_RSHIFT)) {
			speed = 25.5f;
		}
		else {
			speed = 4.5f;
		}

		// camera translation
		if (Input::GetKeyHeld(KEY_D)) {
			Vector3d right = SceneManager::GetMainCamera()->GetTransform()->GetRight();
			right *= speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(right);
		}
		else if (Input::GetKeyHeld(KEY_A)) {
			Vector3d left = SceneManager::GetMainCamera()->GetTransform()->GetRight();
			left *= -speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(left);
		}
		else if (Input::GetKeyHeld(KEY_W)) {
			Vector3d forward = SceneManager::GetMainCamera()->GetTransform()->GetForward();
			forward *= speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(forward);
		}
		else if (Input::GetKeyHeld(KEY_S)) {
			Vector3d backward = SceneManager::GetMainCamera()->GetTransform()->GetForward();
			backward *= -speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(backward);
		}
		else if (Input::GetKeyHeld(KEY_Q)) {
			Vector3d down = SceneManager::GetMainCamera()->GetTransform()->GetUp();
			down *= -speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(down);
		}
		else if (Input::GetKeyHeld(KEY_E)) {
			Vector3d up = SceneManager::GetMainCamera()->GetTransform()->GetUp();
			up *= speed * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(up);
		}

		// camera rotation
		if (Input::GetMouseButtonHeld(MOUSE_RIGHT)) {
			rotationY = Input::GetMouseDeltaPosition().x * sensitivity * Time::DeltaTime();
			rotationX = Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();

			SceneManager::GetMainCamera()->GetTransform()->Rotate(Vector3d::up, -rotationY);
			SceneManager::GetMainCamera()->GetTransform()->Rotate(SceneManager::GetMainCamera()->GetTransform()->GetRight(), -rotationX);
		}
	}
}