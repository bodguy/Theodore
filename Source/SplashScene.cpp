#include "SplashScene.h"

namespace Quark {
	const float Verts[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

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

	SplashScene::SplashScene() : Scene("SplashScene"), program(), vao() {

	}

	SplashScene::~SplashScene() {

	}

	void SplashScene::OnAwake() {
		GameObject* g = new GameObject("asd");
		Attach(g);
		trans = g->GetTransform();
		trans->SetPosition(Vector3d(0.f, -0.5f, 0.f));
		trans->SetScale(Vector3d(20.f, 1.f, 20.f));

		Shader* vs = AssetManager::RequestShader("Core/Shaders/light/vs.glsl", VertexShader);
		Shader* fs = AssetManager::RequestShader("Core/Shaders/light/fs.glsl", FragmentShader);
		program.AttachShader(*vs);
		program.AttachShader(*fs);
		program.Link();

		InputStream vertexData;
		int stride = 6;
		vtxCount = sizeof(Verts) / sizeof(Verts[0]) / stride;
		for (int i = 0; i < vtxCount; i++) {
			vertexData.Vec3(Vector3d(Verts[i * stride], Verts[i * stride + 1], Verts[i * stride + 2]));
			vertexData.Vec3(Vector3d(Verts[i * stride + 3], Verts[i * stride + 4], Verts[i * stride + 5]));
		}
		Buffer vertexBuffer(BufferVertex);
		vertexBuffer.Data(vertexData.Pointer(), vertexData.Size(), StaticDraw);

		vao.BindAttribute(program.GetAttribute("position"), vertexBuffer, 3, 2 * sizeof(Vector3d), 0);
		vao.BindAttribute(program.GetAttribute("normal"), vertexBuffer, 3, 2 * sizeof(Vector3d), sizeof(Vector3d));

		Camera::GetMainCamera()->GetTransform().SetPosition(Vector3d(0.f, 2.f, 5.f));
	}

	void SplashScene::OnUpdate() {
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			Platform::GetInstance()->Quit();
		}

		if (Input::GetKeyDown(KEY_1)) { // setting ortho or perspective
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

		// camera rotation
		if (Input::GetMouseButtonHeld(MOUSE_RIGHT)) {
			// camera translation
			if (Input::GetKeyHeld(KEY_D)) {
				Vector3d right = Camera::GetMainCamera()->GetTransform().GetRight();
				right *= speed * Time::DeltaTime();
				Camera::GetMainCamera()->GetTransform().Translate(right);
			}
			else if (Input::GetKeyHeld(KEY_A)) {
				Vector3d right = Camera::GetMainCamera()->GetTransform().GetRight();
				right *= -speed * Time::DeltaTime();
				Camera::GetMainCamera()->GetTransform().Translate(right);
			}
			else if (Input::GetKeyHeld(KEY_W)) {
				Vector3d forward = Camera::GetMainCamera()->GetTransform().GetForward();
				forward *= speed * Time::DeltaTime();
				Camera::GetMainCamera()->GetTransform().Translate(forward);
			}
			else if (Input::GetKeyHeld(KEY_S)) {
				Vector3d forward = Camera::GetMainCamera()->GetTransform().GetForward();
				forward *= -speed * Time::DeltaTime();
				Camera::GetMainCamera()->GetTransform().Translate(forward);
			}
			else if (Input::GetKeyHeld(KEY_Q)) {
				Vector3d up = Camera::GetMainCamera()->GetTransform().GetUp();
				up *= speed * Time::DeltaTime();
				Camera::GetMainCamera()->GetTransform().Translate(up);
			}
			else if (Input::GetKeyHeld(KEY_E)) {
				Vector3d up = Camera::GetMainCamera()->GetTransform().GetUp();
				up *= -speed * Time::DeltaTime();
				Camera::GetMainCamera()->GetTransform().Translate(up);
			}

			rotationY += Input::GetMouseDeltaPosition().x * sensitivity * Time::DeltaTime();
			//rotationY = Math::Clamp(rotationY, minimumY, maximumY);

			rotationX += Input::GetMouseDeltaPosition().y * sensitivity * Time::DeltaTime();
			//rotationX = Math::Clamp(rotationX, minimumX, maximumX);

			Camera::GetMainCamera()->GetTransform().SetEulerAngles(Vector3d(-rotationX, -rotationY, 0.f));
		}
	}

	void SplashScene::OnRender() {
		//gizmo.Render();

		program.Use();
		//trans.Rotate(Vector3d(0.f , 1.f ,0.f), 20 * Time::DeltaTime());
		//trans.Rotate(Vector3d(1.f, 0.f, 0.f), 20 * Time::DeltaTime());
		program.SetUniform(program.GetUniform("model"), trans->GetLocalToWorldMatrix());
		program.SetUniform(program.GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
		program.SetUniform(program.GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
		program.SetUniform(program.GetUniform("viewPos"), Camera::GetMainCamera()->GetTransform().GetPosition());
		program.SetUniform(program.GetUniform("lightPos"), Vector3d(0.f, 10.f, 0.f));
		program.SetUniform(program.GetUniform("objectColor"), Color::white);
		Graphics::DrawArrays(vao, Triangles, 0, vtxCount);
		program.UnUse();
	}
}