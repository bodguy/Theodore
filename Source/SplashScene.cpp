#include "SplashScene.h"

namespace Quark {
	SplashScene::SplashScene() : Scene("SplashScene") {
		speed = 4.5f;
		rotationY = 0.f;
		rotationX = 0.f;
		sensitivity = 8.0f;
		moveSensitivity = 0.5f;

		minimumX = -360.f;
		maximumX = 360.f;

		minimumY = -60.f;
		maximumY = 60.f;
		isOrtho = false;
	}

	SplashScene::~SplashScene() {

	}

	void SplashScene::OnAwake() {
		gizmo = new GameObject("gizmo", this);
		gizmo->AddComponent<Gizmo>(Enumeration::TranslationGizmo);

		cube = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
		cube->GetTransform()->SetLocalPosition(Vector3d(0.f, 1.f, 0.f));
		cube->GetTransform()->Rotate(Vector3d(0.f, 0.f, 1.f), -90.f, World);

		cube2 = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
		cube2->GetTransform()->SetParent(cube);
		cube2->GetTransform()->SetLocalPosition(Vector3d(4.f, 1.f, 0.f));

		cube3 = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
		cube3->GetTransform()->SetParent(cube2);
		cube3->GetTransform()->SetLocalPosition(Vector3d(8.f, 1.f, 0.f));

		camera = new GameObject("secondCamera", this);
		camera->AddComponent<Camera>();
		camera->GetTransform()->SetPosition(Vector3d(0.f, 5.f, 5.f));

		//Texture2D* tex = AssetManager::RequestTexture("Contents/sprite.png", TextureFormat::RGBA32, Color::white);
		//test = new GameObject("test", this);
		//rend = test->AddComponent<SpriteRenderer>();
		//rend->SetSprite(Sprite::Create(tex));

		//Mesh* mesh = new Mesh();
		//BoneWeight* weights = new BoneWeight[4];
		//weights[0].boneIndex0 = 0;
		//weights[0].weight0 = 1.f;
		//weights[1].boneIndex0 = 0;
		//weights[1].weight0 = 1.f;
		//weights[2].boneIndex0 = 1;
		//weights[2].weight0 = 1.f;
		//weights[3].boneIndex0 = 1;
		//weights[3].weight0 = 1.f;
		//mesh->SetBoneWeight(weights);

		SceneManager::GetMainCamera()->GetTransform()->SetPosition(Vector3d(0.f, 2.f, 10.f));
		//Graphics::SetPolygonMode(FillMode::WireFrame);
	}

	void SplashScene::OnUpdate() {
		ObjectUpdate();
		CameraUpdate();
	}

	void SplashScene::ObjectUpdate() {
		Transform* trans = cube->GetTransform();
		if (Input::GetKeyHeld(KEY_0)) {
			trans->SetLocalPosition(Vector3d::zero);
			trans->SetLocalRotation(Quaternion::identity);
			trans->SetLocalScale(Vector3d::one);
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT) && Input::GetKeyHeld(KEY_LCTRL)) {
			Vector3d right = trans->GetRight();
			right *= Input::GetMouseDeltaPosition().y * Time::DeltaTime() * 20.f;
			trans->Rotate(-right);

			Vector3d up = trans->GetUp();
			up *= Input::GetMouseDeltaPosition().x * Time::DeltaTime() * 20.f;
			trans->Rotate(-up);
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT) && Input::GetKeyHeld(KEY_TAB)) {
			Vector3d right = Vector3d::right;
			right *= Input::GetMouseDeltaPosition().x * Time::DeltaTime();
			trans->Translate(right);

			Vector3d up = Vector3d::up;
			up *= Input::GetMouseDeltaPosition().y * Time::DeltaTime();
			trans->Translate(-up);
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT) && Input::GetKeyHeld(KEY_LALT)) {
			Vector3d right = Vector3d::right;
			right *= Input::GetMouseDeltaPosition().x * Time::DeltaTime();
			trans->Scale(right);

			Vector3d up = Vector3d::up;
			up *= Input::GetMouseDeltaPosition().y * Time::DeltaTime();
			trans->Scale(up);
		}
	}

	void SplashScene::CameraUpdate() {
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}

		if (Input::GetKeyDown(KEY_2)) { // setting ortho or perspective
			isOrtho = !isOrtho;
			SceneManager::GetMainCamera()->SetOrthographic(isOrtho);
		} 
		else if (Input::GetKeyDown(KEY_3)) {
			SceneManager::SetCurrentCamera();
		}
		else if (Input::GetKeyDown(KEY_4)) {
			SceneManager::SetMainCamera(camera->GetComponent<Camera>());
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT) && !Input::GetKeyHeld(KEY_LCTRL) && !Input::GetKeyHeld(KEY_TAB) && !Input::GetKeyHeld(KEY_LALT)) {
			Vector3d right = SceneManager::GetMainCamera()->GetTransform()->GetRight();
			right *= Input::GetMouseDeltaPosition().x * moveSensitivity * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(right);

			Vector3d up = SceneManager::GetMainCamera()->GetTransform()->GetUp();
			up *= Input::GetMouseDeltaPosition().y * moveSensitivity * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(-up);
		}

		if (Input::GetKeyHeld(KEY_LSHIFT) || Input::GetKeyHeld(KEY_RSHIFT)) {
			speed = 13.5f;
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
			rotationY = Math::Clamp(rotationY, minimumY, maximumY);

			rotationX = Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();
			rotationX = Math::Clamp(rotationX, minimumX, maximumX);

			SceneManager::GetMainCamera()->GetTransform()->Rotate(Vector3d::up, -rotationY);
			SceneManager::GetMainCamera()->GetTransform()->Rotate(SceneManager::GetMainCamera()->GetTransform()->GetRight(), -rotationX);
		}
	}
}