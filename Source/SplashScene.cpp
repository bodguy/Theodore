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
		isWire = false;
		camChange = false;
		skyChange = false;
	}

	SplashScene::~SplashScene() {
		
	}

	void SplashScene::OnAwake() {
		GameObject* gizmo = new GameObject("gizmo", this);
		gizmo->AddComponent<Gizmo>(GizmoType::Translation);

		plane = GameObject::CreatePrimitive(PrimitiveType::Plane, this);
		plane->GetTransform()->SetLocalPosition(Vector3d(0.f, -1.f, 0.f));

		sphere = GameObject::CreatePrimitive(PrimitiveType::Sphere, this);
		cylinder = GameObject::CreatePrimitive(PrimitiveType::Cylinder, this);
		cylinder->GetTransform()->SetLocalPosition(Vector3d(-3.f, 0.f, 0.f));
		torus = GameObject::CreatePrimitive(PrimitiveType::Torus, this);
		torus->GetTransform()->SetLocalPosition(Vector3d(6.f, 0.f, 0.f));
		cone = GameObject::CreatePrimitive(PrimitiveType::Cone, this);
		cone->GetTransform()->SetLocalPosition(Vector3d(9.f, 0.f, 0.f));

		dragon = new GameObject("dragon", this);
		dragon->GetTransform()->SetLocalPosition(Vector3d(-6.f, 0.f, 0.f));
		dragon->GetTransform()->SetLocalScale(Vector3d(0.2f, 0.2f, 0.2f));
		Mesh* obj = AssetManager::RequestMesh("Contents/dragon.obj");
		Material* mat = new Material(Shader::Find("Standard"));
		MeshRenderer* rend1 = dragon->AddComponent<MeshRenderer>();
		rend1->SetMaterial(mat);
		rend1->SetMesh(obj);

		for (int i = 0; i < 10; i++) {
			GameObject* gg = new GameObject("gg" + std::to_string(i), this);
			gg->GetTransform()->SetLocalPosition(Vector3d(0.f, 3.f * i, 0.f));
			gg->GetTransform()->SetLocalScale(Vector3d(0.2f, 0.2f, 0.2f));
			MeshRenderer* rr = gg->AddComponent<MeshRenderer>();
			Material* m = new Material(Shader::Find("Standard"));
			rr->SetMaterial(m);
			rr->SetMesh(obj);
		}

		FrameBuffer* frameBuffer = new FrameBuffer(Platform::GetWidth(), Platform::GetHeight());
		frameBuffer->AttachTexture(AssetManager::RequestTexture("raw", Platform::GetWidth(), Platform::GetHeight(), TextureFormat::RGBA32, nullptr), Attachment::Color0);
		frameBuffer->Create(true);

		cube = new GameObject("cube", this);
		cube->GetTransform()->SetLocalPosition(Vector3d(3.f, 0.f, 0.f));
		Mesh* mesh = new Mesh();
		mesh = ShapeGenerator::GenerateCube();
		Material* mat2 = new Material(Shader::Find("Standard"));
		//mat->renderTexture = frameBuffer;
		mat2->texture0 = AssetManager::RequestTexture("Contents/container2.png", TextureFormat::RGBA32);
		mat2->texture1 = AssetManager::RequestTexture("Contents/container2_specular.png", TextureFormat::RGBA32);
		MeshRenderer* rend2 = cube->AddComponent<MeshRenderer>();
		rend2->SetMaterial(mat2);
		rend2->SetMesh(mesh);

		GameObject* lightObject[] = {
			new GameObject("light1", this),
			new GameObject("light2", this),
			new GameObject("light3", this),
			new GameObject("light4", this),
			new GameObject("light5", this),
			new GameObject("light6", this)
		};

		light1 = lightObject[0]->AddComponent<Light>(LightType::PointLight);
		light2 = lightObject[1]->AddComponent<Light>(LightType::PointLight);
		light3 = lightObject[2]->AddComponent<Light>(LightType::PointLight);
		light4 = lightObject[3]->AddComponent<Light>(LightType::SpotLight);
		light5 = lightObject[4]->AddComponent<Light>(LightType::SpotLight);
		light6 = lightObject[5]->AddComponent<Light>(LightType::SpotLight);

		light1->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 0.f));
		light1->ambient = Color::red;
		light1->diffuse= Color::red;
		light1->specular = Color::red;
		
		light2->GetTransform()->SetPosition(Vector3d(10.f, 0.f, 0.f));
		light2->ambient = Color::green;
		light2->diffuse = Color::green;
		light2->specular = Color::green;
		
		light3->GetTransform()->SetPosition(Vector3d(-10.f, 0.f, 0.f));
		light3->ambient = Color::blue;
		light3->diffuse = Color::blue;
		light3->specular = Color::blue;

		light4->ambient = Color::white;
		light4->diffuse = Color::white;
		light4->specular = Color::white;

		light5->ambient = Color::orange;
		light5->diffuse = Color::orange;
		light5->specular = Color::orange;

		light6->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 15.f));
		light6->ambient = Color::blue;
		light6->diffuse = Color::blue;
		light6->specular = Color::blue;

		cam2 = new GameObject("cam2", this);
		Camera* c = cam2->AddComponent<Camera>();
		c->SetRenderTexture(frameBuffer);
		c->GetTransform()->SetPosition(Vector3d(0.f, 0.f, 20.f));

		skybox = new GameObject("Skybox", this);
		CubemapRenderer* cubemap = skybox->AddComponent<CubemapRenderer>();
		AssetManager::RequestTexture(cubemap, "Contents/swedish/posx.jpg", TextureFormat::RGBA32, CubemapFace::PositiveX); // Right
		AssetManager::RequestTexture(cubemap, "Contents/swedish/negx.jpg", TextureFormat::RGBA32, CubemapFace::NegativeX); // Left
		AssetManager::RequestTexture(cubemap, "Contents/swedish/posy.jpg", TextureFormat::RGBA32, CubemapFace::PositiveY); // Top
		AssetManager::RequestTexture(cubemap, "Contents/swedish/negy.jpg", TextureFormat::RGBA32, CubemapFace::NegativeY); // Bottom
		AssetManager::RequestTexture(cubemap, "Contents/swedish/posz.jpg", TextureFormat::RGBA32, CubemapFace::PositiveZ); // Back
		AssetManager::RequestTexture(cubemap, "Contents/swedish/negz.jpg", TextureFormat::RGBA32, CubemapFace::NegativeZ); // Front

		SceneManager::GetMainCamera()->GetTransform()->SetPosition(Vector3d(0.f, 2.f, 10.f));
	}

	void SplashScene::OnUpdate() {
		ObjectUpdate();
		CameraUpdate();

		if (Input::GetMouseButtonHeld(MOUSE_LEFT) && Input::GetKeyHeld(KEY_SPACE)) {
			Vector3d right = Vector3d::right;
			right *= Input::GetMouseDeltaPosition().x * Time::DeltaTime();
			light1->GetTransform()->Translate(right);

			Vector3d up = Vector3d::up;
			up *= Input::GetMouseDeltaPosition().y * Time::DeltaTime();
			light1->GetTransform()->Translate(-up);
		}

		light4->GetTransform()->Rotate(Vector3d::up * Time::DeltaTime() * 20.f);
		light5->GetTransform()->Rotate(Vector3d::up * Time::DeltaTime() * 10.f);
	}

	void SplashScene::ObjectUpdate() {
		cube->GetTransform()->Rotate(Vector3d(1.f, 1.f, 0.f), Time::DeltaTime() * 20.f);

		Transform* trans = dragon->GetTransform();
		if (Input::GetKeyHeld(KEY_0)) {
			trans->SetLocalPosition(Vector3d::zero);
			trans->SetLocalRotation(Quaternion::identity);
			trans->SetLocalScale(Vector3d::one);
		}
		else if (Input::GetKeyDown(KEY_1)) {
			isWire = !isWire;
		}

		if (isWire) {
			Graphics::SetPolygonMode(FillMode::WireFrame);
		}
		else {
			Graphics::SetPolygonMode(FillMode::Solid);
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
			camChange = !camChange;

			if(camChange) {
				SceneManager::SetMainCamera(cam2->GetComponent<Camera>());
			}
			else {
				SceneManager::SetCurrentCamera();
			}
			
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT) && !Input::GetKeyHeld(KEY_LCTRL) && !Input::GetKeyHeld(KEY_TAB) && !Input::GetKeyHeld(KEY_LALT) && !Input::GetKeyHeld(KEY_SPACE)) {
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