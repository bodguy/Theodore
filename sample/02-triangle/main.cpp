// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "TheodoreEngine.h"

using namespace Theodore;

class MyTriangleScene : public Scene {
public:
	MyTriangleScene() : Scene("MyTriangleScene") {
		speed = 20.f;
		rotationY = 0.f;
		rotationX = 0.f;
		sensitivity = 15.0f;
		moveSensitivity = 0.5f;
		fieldOfView = 60.f;
	}
  virtual ~MyTriangleScene() override {}

  virtual void OnAwake() {
		SceneManager::GetMainCamera()->GetTransform()->Translate(Vector3d(0.f, 0.f, 10.f));

		Font* fontAsset = AssetManager::RequestFont(Application::GetResourcePath() + "arial.ttf", 0, 48);
		GameObject* GUIFont = new GameObject("GUI font", this);
		FontRenderer* fr = GUIFont->AddComponent<FontRenderer>(fontAsset);
		fr->SetColor(Color::red);
		fr->SetText("AB");
		fr->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 0.f));

		GameObject* sprites = new GameObject("sprite", this);
		SpriteRenderer* sr = sprites->AddComponent<SpriteRenderer>();
		Sprite* sprite = Sprite::Create(AssetManager::RequestTexture(Application::GetResourcePath() + "dragon.png", TextureFormat::RGBA32));
		sr->SetSprite(sprite);
		sprites->GetTransform()->SetLocalScale(Vector3d(0.1f, 0.1f, 0.1f));
	}

  virtual void OnStart() {
    Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
		Input::AddAxis("Forward", new InputHandler(KEY_Q, KEY_E, 0.01f));
  }

  virtual void OnUpdate() {
		CameraUpdate();
	}

	void CameraUpdate() {
		Graphics::DrawFrustum(SceneManager::GetMainCamera(), Color::red);
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}

		Camera* cam = SceneManager::GetMainCamera();
		Transform* trans = cam->GetTransform();

		if (Input::GetMouseButtonHeld(MOUSE_MIDDLE)) {
			Vector3d right = SceneManager::GetMainCamera()->GetTransform()->GetRight();
			right *= Input::GetMouseDeltaPosition().x * moveSensitivity * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(right);

			Vector3d up = SceneManager::GetMainCamera()->GetTransform()->GetUp();
			up *= Input::GetMouseDeltaPosition().y * moveSensitivity * Time::DeltaTime();
			SceneManager::GetMainCamera()->GetTransform()->Translate(up);
		}

		if (Input::GetKeyHeld(KEY_LSHIFT) || Input::GetKeyHeld(KEY_RSHIFT)) {
			speed = 100.f;
		} else {
			speed = 20.f;
		}

		// camera translation
		Vector3d right = trans->GetRight();
		right *= Input::GetAxis("Horizontal") * speed * Time::DeltaTime();
		trans->Translate(right);

		Vector3d forward = trans->GetForward();
		forward *= Input::GetAxis("Vertical") * speed * Time::DeltaTime();
		trans->Translate(forward);

		Vector3d up = trans->GetUp();
		up *= Input::GetAxis("Forward") * speed * Time::DeltaTime();
		trans->Translate(up);

		// camera rotation
		if (Input::GetMouseButtonHeld(MOUSE_RIGHT)) {
			// zoom in, out
			Vector3d pos = Input::GetMouseDeltaPosition();
			fieldOfView += Input::GetMousePosition().z;
			cam->SetFieldOfView(fieldOfView);

			rotationY = pos.x * sensitivity * Time::DeltaTime();
			rotationX = pos.y * sensitivity * Time::DeltaTime();

			trans->Rotate(Vector3d::up, -rotationY);
			trans->Rotate(trans->GetRight(), rotationX);
		} else {
			fieldOfView = 60.f;
			cam->ResetFieldOfView();
		}
	}

	float speed;
	float rotationY;
	float rotationX;
	float sensitivity;
	float moveSensitivity;
	float fieldOfView;
};

int main(int argc, char** argv) {
  Application app;
  PlatformContext context;
  context.width = 1280;
  context.height = 720;

  if (app.Initialize(context)) {
		SceneManager::SetActiveScene(SceneManager::CreateScene<MyTriangleScene>("MyTriangleScene"));
    app.Run();
  }

  return 0;
}