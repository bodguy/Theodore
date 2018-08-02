#ifndef MyNewScene_h
#define MyNewScene_h

#include "TheodoreEngine.h"

namespace Theodore {
	class MyNewScene : public Scene {
	public:
		MyNewScene() : Scene("MyNewScene") {

		}

		virtual ~MyNewScene() {

		}

		virtual void OnAwake() {
			cube = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
			trans = cube->GetTransform();
			trans->SetPosition(Vector3d(0.f, 0.f, -4.f));

			GameObject* pointLight = new GameObject("pointLight", this);
			Light* pl = pointLight->AddComponent<Light>(LightType::PointLight);
			pl->ambient = Color::white;
			pl->diffuse = Color::white;
			pl->specular = Color::white;
			pl->GetTransform()->SetLocalPosition(Vector3d(5.f, 0.f, 0.f));
		}

		virtual void OnStart() {
			Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
		}

		virtual void OnUpdate() {
			Graphics::DrawLine(trans->GetPosition(), trans->GetPosition() + trans->GetRight() * 2.f, Color::red);
			Graphics::DrawLine(trans->GetPosition(), trans->GetPosition() + trans->GetUp() * 2.f, Color::blue);
			Graphics::DrawLine(trans->GetPosition(), trans->GetPosition() + trans->GetForward() * 2.f, Color::green);

			Vector3d forward = trans->GetForward();
			forward *= Input::GetAxis("Vertical") * 20.f * Time::DeltaTime();
			trans->Translate(forward);

			trans->Rotate(Vector3d::one, Time::DeltaTime() * 10.f);
		}

	private:
		GameObject * cube;
		Transform* trans;
	};
}

#endif /* MyNewScene_h */