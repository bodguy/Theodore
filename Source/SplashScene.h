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
#include "MyNewScene.h"

namespace Theodore {
	class SplashScene : public Scene {
	public:
		SplashScene() : Scene("SplashScene") {
			speed = 20.f;
			rotationY = 0.f;
			rotationX = 0.f;
			sensitivity = 8.0f;
			moveSensitivity = 0.5f;
			fieldOfView = 60.f;
		}

		virtual ~SplashScene() {}

		virtual void OnAwake() {
			tiger = new GameObject("spriteTest", this);
			tiger->AddComponent<SpriteRenderer>()->SetSprite(
				Sprite::Create(AssetManager::RequestTexture("Contents/sprite.png", TextureFormat::RGBA32, Color(1.f, 1.f, 1.f, 1.f))));
			rigidBody = tiger->AddComponent<RigidBody2D>();

			dragon = new GameObject("test2", tiger, this);
			dragon->AddComponent<SpriteRenderer>()->SetSprite(
				Sprite::Create(AssetManager::RequestTexture("Contents/dragon.png", TextureFormat::RGBA32)));
			dragon->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 0.1f));

			SceneManager::GetMainCamera()->GetTransform()->Translate(Vector3d(0.f, 0.f, 20.f));
		}

		virtual void OnStart() {
			Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
		}

		virtual void OnUpdate() {
			Transform* trans = tiger != nullptr ? tiger->GetTransform() : nullptr;
			Graphics::DrawLine(trans->GetPosition(), trans->GetPosition() + trans->GetRight() * 2.f, Color::red);
			Graphics::DrawLine(trans->GetPosition(), trans->GetPosition() + trans->GetUp() * 2.f, Color::blue);
			Graphics::DrawLine(trans->GetPosition(), trans->GetPosition() + trans->GetForward() * 2.f, Color::green);

			if (Input::GetKeyHeld(KEY_RIGHT)) {
				trans->Translate(trans->GetRight() * Vector3d(0.2f, 0.f, 0.f));
			}
			else if (Input::GetKeyHeld(KEY_LEFT)) {
				trans->Translate(-trans->GetRight() * Vector3d(0.2f, 0.f, 0.f));
			}
			else if (Input::GetKeyHeld(KEY_UP)) {
				trans->Translate(trans->GetUp() * Vector3d(0.f, 0.2f, 0.f));
			}
			else if (Input::GetKeyHeld(KEY_DOWN)) {
				trans->Translate(-trans->GetUp() * Vector3d(0.f, 0.2f, 0.f));
			}
			//else if (Input::GetKeyDown(KEY_1)) {
			//	rigidBody->AddForce(Vector2d(0.f, 2.f));
			//}
			else if (Input::GetKeyDown(KEY_3)) {
				rigidBody->AddTorque(2.f);
			}
			else if (Input::GetKeyHeld(KEY_1)) {
				trans->Rotate(Vector3d::forward, Time::DeltaTime() * 20.f);
			}
			else if (Input::GetKeyHeld(KEY_2)) {
				trans->Rotate(Vector3d::backward, Time::DeltaTime() * 20.f);
			}

			if (Input::GetKeyDown(KEY_0)) {
				SceneManager::SetActiveScene(SceneManager::CreateScene<MyNewScene>("MyNewScene"));
			}

			CameraUpdate();
		}

		void CameraUpdate() {
			if (Input::GetKeyDown(KEY_ESCAPE)) {
				Platform::GetInstance()->Quit();
			}

			if (Input::GetKeyDown(KEY_BACKSPACE)) {
				Object::Destroy(tiger);
			}

			Camera* cam = SceneManager::GetMainCamera();
			Transform* trans = cam->GetTransform();

			if (Input::GetMouseButtonHeld(MOUSE_MIDDLE)) {
				Vector3d right = SceneManager::GetMainCamera()->GetTransform()->GetRight();
				right *= Input::GetMouseDeltaPosition().x * moveSensitivity * Time::DeltaTime();
				SceneManager::GetMainCamera()->GetTransform()->Translate(right);

				Vector3d up = SceneManager::GetMainCamera()->GetTransform()->GetUp();
				up *= Input::GetMouseDeltaPosition().y * moveSensitivity * Time::DeltaTime();
				SceneManager::GetMainCamera()->GetTransform()->Translate(-up);
			}

			if (Input::GetKeyHeld(KEY_LSHIFT) || Input::GetKeyHeld(KEY_RSHIFT)) {
				speed = 100.f;
			}
			else {
				speed = 60.f;
			}

			// camera translation
			Vector3d right = trans->GetRight();
			right *= Input::GetAxis("Horizontal") * speed * Time::DeltaTime();
			trans->Translate(right);

			Vector3d forward = trans->GetForward();
			forward *= Input::GetAxis("Vertical") * speed * Time::DeltaTime();
			trans->Translate(forward);

			if (Input::GetKeyHeld(KEY_Q)) {
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
				Vector3d pos = Input::GetMousePosition();
				if (pos.z < 0.f) {
					fieldOfView += 3;
				}
				else if (pos.z > 0.f) {
					fieldOfView -= 3;
				}
				cam->SetFieldOfView(fieldOfView);

				rotationY = Input::GetMouseDeltaPosition().x * sensitivity * Time::DeltaTime();
				rotationX = Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();

				trans->Rotate(Vector3d::up, -rotationY);
				trans->Rotate(trans->GetRight(), -rotationX);
			}
			else {
				fieldOfView = 60.f;
				cam->ResetFieldOfView();
			}
		}

		GameObject* tiger, *dragon;
		RigidBody2D* rigidBody;
		float speed;
		float rotationY;
		float rotationX;
		float sensitivity;
		float moveSensitivity;
		float fieldOfView;
	};
}

#endif /* SplashScene_h */