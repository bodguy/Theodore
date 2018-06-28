#include "SplashScene.h"

namespace Quark {
	SplashScene::SplashScene() : Scene("SplashScene") {
		speed = 4.5f;
		rotationY = 0.f;
		rotationX = 0.f;
		sensitivity = 8.0f;
		moveSensitivity = 0.5f;
	}

	SplashScene::~SplashScene() {

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
		rend->SetMesh(AssetManager::RequestMesh("Contents/model/budda.obj"));
		monkey->GetTransform()->SetLocalScale(Vector3d(60.f, 60.f, 60.f));
		monkey->GetTransform()->Translate(Vector3d(0.f, -10.f, 0.f), Space::World);

		cube = new GameObject("cube", this);
		cube->GetTransform()->SetLocalPosition(Vector3d(5.f, 0.f, 0.f));
		MeshRenderer* rend2 = cube->AddComponent<MeshRenderer>();
		Material* mat2 = new Material(Shader::Find("Phong"));
		mat2->texture0 = AssetManager::RequestTexture("Contents/BlueGrid.png", TextureFormat::RGBA32);
		Mesh* mesh = ShapeGenerator::GenerateCube();
		rend2->SetMaterial(mat2);
		rend2->SetMesh(mesh);

		GameObject* pointLight = new GameObject("pointLight", this);
		Light* pl = pointLight->AddComponent<Light>(LightType::PointLight);
		pl->ambient = Color::white;
		pl->diffuse = Color::white;
		pl->specular = Color::white;
		pl->GetTransform()->SetLocalPosition(Vector3d::zero);
		
		SceneManager::GetMainCamera()->GetTransform()->SetLocalPosition(Vector3d(0.f, 5.f, 20.f));

		float distanceZ = -10.f;
		Vector3d distanceFromCamera = Vector3d(SceneManager::GetMainCamera()->GetTransform()->GetLocalPosition().x,
			SceneManager::GetMainCamera()->GetTransform()->GetLocalPosition().y, distanceZ);
		planes = new Plane(Vector3d::forward, distanceFromCamera);
	}

	void SplashScene::OnUpdate() {
		monkey->GetTransform()->Rotate(Vector3d(0.f, 1.f, 0.f), Time::DeltaTime() * 40.f);

		// ray casting
		if (Input::GetMouseButtonHeld(MOUSE_LEFT)) {
			Ray ray = SceneManager::GetMainCamera()->ScreenPointToRay(Input::GetMousePosition());
			float enter = 0.f;

			if (planes->Raycast(ray, &enter)) {
				hitPoint = ray.GetPoint(enter);
				Debug::Log(hitPoint);
				camPos = SceneManager::GetMainCamera()->GetTransform()->GetLocalPosition();
				camPos.z += 0.2f;
				monkey->GetTransform()->SetLocalPosition(hitPoint);
			}
		}

		if (Input::GetKeyDown(KEY_1)) {
			SceneManager::GetMainCamera()->SetOrthographic(true);
		}
		
		if(Input::GetKeyDown(KEY_2)) {
			SceneManager::GetMainCamera()->SetOrthographic(false);
		}

		Graphics::DrawLine(camPos, hitPoint, Color::green);
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