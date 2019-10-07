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
    SplashScene() : Scene("SplashScene") {
      speed = 20.f;
      rotationY = 0.f;
      rotationX = 0.f;
      sensitivity = 15.0f;
      moveSensitivity = 0.5f;
      fieldOfView = 60.f;
    }

    virtual ~SplashScene() {}

    virtual void OnAwake() {
      GameObject* pointLight = GameObject::CreatePrimitive(PrimitiveType::Sphere, this);
      pointLight->SetTag("light");
      Light* pl = pointLight->AddComponent<Light>(LightType::PointLight);
      pl->ambient = Color::white;
      pl->diffuse = Color::white;
      pl->specular = Color::white;
      pl->GetTransform()->SetPosition(Vector3d(5.f, 0.f, 0.f));
      pl->GetTransform()->SetLocalScale(Vector3d(0.2f, 0.2f, 0.2f));

      GameObject* pointLight2 = GameObject::CreatePrimitive(PrimitiveType::Sphere, this);
      pointLight2->SetTag("light");
      Light* pl2 = pointLight2->AddComponent<Light>(LightType::PointLight);
      pl2->ambient = Color::red;
      pl2->diffuse = Color::red;
      pl2->specular = Color::red;
      pl2->GetTransform()->SetPosition(Vector3d(0.f, 5.f, 0.f));
      pl2->GetTransform()->SetLocalScale(Vector3d(0.2f, 0.2f, 0.2f));

      GameObject* pointLight3 = GameObject::CreatePrimitive(PrimitiveType::Sphere, this);
      pointLight3->SetTag("light");
      Light* pl3 = pointLight3->AddComponent<Light>(LightType::PointLight);
      pl3->ambient = Color::green;
      pl3->diffuse = Color::green;
      pl3->specular = Color::green;
      pl3->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 5.f));
      pl3->GetTransform()->SetLocalScale(Vector3d(0.2f, 0.2f, 0.2f));

      GameObject* cube = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
      cube->GetTransform()->SetPosition(Vector3d(0.f, 0.f, -5.f));

      SceneManager::GetMainCamera()->GetTransform()->Translate(Vector3d(0.f, 0.f, 10.f));
    }

    virtual void OnStart() {
      Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
      Input::AddAxis("Forward", new InputHandler(KEY_Q, KEY_E, 0.01f));

      auto v = FindGameObjectsWithTag("light");
      Debug::Log(v.size());

      auto g = Find("Sphere");
      Debug::Log(g->GetTransform()->GetPosition());
    }

    virtual void OnUpdate() { CameraUpdate(); }

    void CameraUpdate() {
      Graphics::DrawFrustum(SceneManager::GetMainCamera(), Color::red);
      Graphics::DrawSphere(Vector3d::zero, 3.f, Color::green);
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
        SceneManager::GetMainCamera()->GetTransform()->Translate(-up);
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
        fieldOfView += pos.z;
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
}  // namespace Theodore

#endif /* SplashScene_h */
