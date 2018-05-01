#include "SplashScene.h"
#include "QuarkEngine.h"

namespace Quark {
	float speed = 4.5f;
	float rotationY = 0.f;
	float rotationX = 0.f;
	float sensitivity = 8.0f;
	float moveSensitivity = 0.5f;

	float minimumX = -360.f;
	float maximumX = 360.f;

	float minimumY = -60.f;
	float maximumY = 60.f;
	bool isOrtho = false;

	SplashScene::SplashScene() : Scene("SplashScene") {

	}

	SplashScene::~SplashScene() {

	}

	void SplashScene::OnAwake() {
		GameObject* gizmo = new GameObject("gizmo", this);
		gizmo->AddComponent<Gizmo>(Enumeration::TranslationGizmo);

		GameObject* cube = GameObject::CreatePrimitive(Enumeration::CubePrimitive, this);
		trans = cube->GetTransform();
		trans->SetPosition(Vector3d(0.f, 1.f, 0.f));

		Camera::GetMainCamera()->GetTransform().SetPosition(Vector3d(0.f, 2.f, 5.f));
	}

	void SplashScene::OnUpdate() {
		trans->Rotate(Vector3d(0.f, 1.f, 0.), 50 * Time::DeltaTime());

		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}

		if (Input::GetKeyDown(KEY_2)) { // setting ortho or perspective
			isOrtho = !isOrtho;
			Camera::GetMainCamera()->SetOrthographic(isOrtho);
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT)) {
			Vector3d right = Camera::GetMainCamera()->GetTransform().GetRight();
			right *= Input::GetMouseDeltaPosition().x * moveSensitivity * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(right);

			Vector3d up = Camera::GetMainCamera()->GetTransform().GetUp();
			up *= Input::GetMouseDeltaPosition().y * -moveSensitivity * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(up);
		}

		if (Input::GetKeyHeld(KEY_LSHIFT) || Input::GetKeyHeld(KEY_RSHIFT)) {
			speed = 13.5f;
		}
		else {
			speed = 4.5f;
		}

		// camera translation
		if (Input::GetKeyHeld(KEY_D)) {
			Vector3d right = Camera::GetMainCamera()->GetTransform().GetRight();
			right *= speed * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(right);
		}
		else if (Input::GetKeyHeld(KEY_A)) {
			Vector3d left = Camera::GetMainCamera()->GetTransform().GetRight();
			left *= -speed * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(left);
		}
		else if (Input::GetKeyHeld(KEY_W)) {
			Vector3d forward = Camera::GetMainCamera()->GetTransform().GetForward();
			forward *= speed * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(forward);
		}
		else if (Input::GetKeyHeld(KEY_S)) {
			Vector3d backward = Camera::GetMainCamera()->GetTransform().GetForward();
			backward *= -speed * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(backward);
		}
		else if (Input::GetKeyHeld(KEY_Q)) {
			Vector3d down = Camera::GetMainCamera()->GetTransform().GetUp();
			down *= -speed * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(down);
		}
		else if (Input::GetKeyHeld(KEY_E)) {
			Vector3d up = Camera::GetMainCamera()->GetTransform().GetUp();
			up *= speed * Time::DeltaTime();
			Camera::GetMainCamera()->GetTransform().Translate(up);
		}
		else if (Input::GetKeyDown(KEY_1)) {
			Debug::Log(Input::GetMousePosition());
		}

		// camera rotation
		if (Input::GetMouseButtonHeld(MOUSE_RIGHT)) {
			rotationY = Input::GetMouseDeltaPosition().x * sensitivity * Time::DeltaTime();
			rotationY = Math::Clamp(rotationY, minimumY, maximumY);

			rotationX = Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();
			rotationX = Math::Clamp(rotationX, minimumX, maximumX);

			Camera::GetMainCamera()->GetTransform().Rotate(Vector3d::up, -rotationY);
			Camera::GetMainCamera()->GetTransform().Rotate(Camera::GetMainCamera()->GetTransform().GetRight(), -rotationX);
		}
	}
}