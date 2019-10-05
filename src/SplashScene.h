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
      cube = GameObject::CreatePrimitive(PrimitiveType::Cube, this);
      trans = cube->GetTransform();
      trans->SetPosition(Vector3d(0.f, 0.f, -4.f));
      cube->AddComponent<BoxCollider>();

      GameObject* skybox = new GameObject("skybox", this);
      CubemapRenderer* cubemap = skybox->AddComponent<CubemapRenderer>();
      AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/posx.jpg", TextureFormat::RGBA32,
                                   CubemapFace::PositiveX); // Right
      AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/negx.jpg", TextureFormat::RGBA32,
                                   CubemapFace::NegativeX); // Left
      AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/posy.jpg", TextureFormat::RGBA32,
                                   CubemapFace::PositiveY); // Top
      AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/negy.jpg", TextureFormat::RGBA32,
                                   CubemapFace::NegativeY); // Bottom
      AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/posz.jpg", TextureFormat::RGBA32,
                                   CubemapFace::PositiveZ); // Back
      AssetManager::RequestTexture(cubemap, Application::GetResourcePath() + "swedish/negz.jpg", TextureFormat::RGBA32,
                                   CubemapFace::NegativeZ); // Front

      GameObject* pointLight = new GameObject("pointLight", this);
      Light* pl = pointLight->AddComponent<Light>(LightType::PointLight);
      pl->ambient = Color::white;
      pl->diffuse = Color::white;
      pl->specular = Color::white;
      pl->GetTransform()->SetLocalPosition(Vector3d(5.f, 0.f, 0.f));

      sprite = new GameObject("sprite", this);
      SpriteRenderer* rend = sprite->AddComponent<SpriteRenderer>();
      rend->SetSprite(Sprite::Create(AssetManager::RequestTexture(Application::GetResourcePath() + "sprite.png", TextureFormat::RGBA32, Color::white)));
      trans2 = sprite->GetTransform();
      trans2->SetLocalScale(Vector3d(0.01f, 0.01f, 0.01f));

      SceneManager::GetMainCamera()->GetTransform()->Translate(Vector3d(0.f, 0.f, 5.f));
    }

    virtual void OnStart() {
      Platform::ChangeTitle(SceneManager::GetActiveScene()->ToString());
      Input::AddAxis("Forward", new InputHandler(KEY_Q, KEY_E, 0.01f));
    }

    virtual void OnUpdate() {
      trans->Rotate(Vector3d::one, Time::DeltaTime() * 100.f);
      trans2->Rotate(Vector3d::forward, Time::DeltaTime() * 40.f);

      if (Input::GetKeyHeld(KEY_LEFT)) {
        trans2->Translate(Vector3d::left * 5.f * Time::DeltaTime());
      } else if (Input::GetKeyHeld(KEY_RIGHT)) {
        trans2->Translate(Vector3d::right * 5.f * Time::DeltaTime());
      } else if (Input::GetKeyHeld(KEY_UP)) {
        trans2->Translate(Vector3d::up * 5.f * Time::DeltaTime());
      } else if (Input::GetKeyHeld(KEY_DOWN)) {
        trans2->Translate(Vector3d::down * 5.f * Time::DeltaTime());
      } else if (Input::GetKeyHeld(KEY_L)) {
        trans2->Scale(Vector3d::one * 0.1f);
      }

      CameraUpdate();
    }

    void CameraUpdate() {
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
        Vector3d pos = Input::GetMousePosition();
        if (pos.z < 0.f) {
          fieldOfView += 3;
        } else if (pos.z > 0.f) {
          fieldOfView -= 3;
        }
        cam->SetFieldOfView(fieldOfView);

        rotationY = Input::GetMouseDeltaPosition().x * sensitivity * Time::DeltaTime();
        rotationX = Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();

        trans->Rotate(Vector3d::up, -rotationY);
        trans->Rotate(trans->GetRight(), rotationX);
      } else {
        fieldOfView = 60.f;
        cam->ResetFieldOfView();
      }
    }

    GameObject *cube, *sprite;
    Transform *trans, *trans2;
    float speed;
    float rotationY;
    float rotationX;
    float sensitivity;
    float moveSensitivity;
    float fieldOfView;
  };
} // namespace Theodore

#endif /* SplashScene_h */
