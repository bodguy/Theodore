#include "QuarkEngine.h"

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

int main() {
	Platform platform;
	Input input;
	Time timer;
	AssetManager manager;
	Camera camera;
	platform.Initialize("Camera Movement", 1024, 800, false, 4, 3, 16);
	platform.SetVSync(true);
	Graphics::Enable(DepthTest);
	TranslateGizmo gizmo;

	Shader vs(VertexShader), fs(FragmentShader);
	File source;
	source.Open("Core/Shaders/light/vs.glsl", Read);
	if (source.IsOpen()) {
		vs.Compile(source.ReadUntilEnd());
		source.Close();
	}
	source.Open("Core/Shaders/light/fs.glsl", Read);
	if (source.IsOpen()) {
		fs.Compile(source.ReadUntilEnd());
		source.Close();
	}
	Program program(vs, fs);

	InputStream vertexData;
	int stride = 6;
	int vtxCount = sizeof(Verts) / sizeof(Verts[0]) / stride;
	for (int i = 0; i < vtxCount; i++) {
		vertexData.Vec3(Vector3d(Verts[i * stride], Verts[i * stride + 1], Verts[i * stride + 2]));
		vertexData.Vec3(Vector3d(Verts[i * stride + 3], Verts[i * stride + 4], Verts[i * stride + 5]));
	}
	Buffer vertexBuffer(BufferVertex);
	vertexBuffer.Data(vertexData.Pointer(), vertexData.Size(), StaticDraw);

	VertexArray vao;
	vao.BindAttribute(program.GetAttribute("position"), vertexBuffer, 3, 2 * sizeof(Vector3d), 0);
	vao.BindAttribute(program.GetAttribute("normal"), vertexBuffer, 3, 2 * sizeof(Vector3d), sizeof(Vector3d));

	Transform trans;
	trans.SetPosition(Vector3d(0.f, 0.f, 0.f));

	float speed = 1.5f;
	float rotationY = 0.f;
	float rotationX = 0.f;
	float sensitivity = 0.15f;

	float minimumX = -360.f;
	float maximumX = 360.f;

	float minimumY = -60.f;
	float maximumY = 60.f;
	bool isOrtho = false;

	while (platform.IsRunning()) {
		platform.Update();
		timer.Update();
		input.Update();

		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}

		// camera translation
		if (Input::GetKeyHeld(KEY_D)) {
			Vector3d right = camera.GetTransform().GetRight();
			right *= speed * Time::DeltaTime();
			camera.GetTransform().Translate(right);
		}
		else if (Input::GetKeyHeld(KEY_A)) {
			Vector3d right = camera.GetTransform().GetRight();
			right *= -speed * Time::DeltaTime();
			camera.GetTransform().Translate(right);
		}
		else if (Input::GetKeyHeld(KEY_W)) {
			Vector3d up = camera.GetTransform().GetUp();
			up *= speed * Time::DeltaTime();
			camera.GetTransform().Translate(up);
		}
		else if (Input::GetKeyHeld(KEY_S)) {
			Vector3d up = camera.GetTransform().GetUp();
			up *= -speed * Time::DeltaTime();
			camera.GetTransform().Translate(up);
		}
		else if (Input::GetKeyDown(KEY_1)) { // setting ortho or perspective
			isOrtho = !isOrtho;
			camera.SetOrthographic(isOrtho);
		}
		else if (Input::GetKeyHeld(KEY_Q)) {
			Vector3d forward = camera.GetTransform().GetForward();
			forward *= speed * Time::DeltaTime();
			camera.GetTransform().Translate(forward);
		}
		else if (Input::GetKeyHeld(KEY_E)) {
			Vector3d forward = camera.GetTransform().GetForward();
			forward *= -speed * Time::DeltaTime();
			camera.GetTransform().Translate(forward);
		}

		// camera rotation
		if (Input::GetMouseButtonHeld(MOUSE_RIGHT)) {
			rotationY += Input::GetMouseDeltaPosition().x * sensitivity;
			//rotationY = Math::Clamp(rotationY, minimumY, maximumY);

			rotationX += Input::GetMouseDeltaPosition().y * sensitivity;
			//rotationX = Math::Clamp(rotationX, minimumX, maximumX);

			camera.GetTransform().SetEulerAngles(Vector3d(-rotationX, -rotationY, 0.f));
		}

		Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), ColorBits | DepthBits);
		gizmo.Render(camera);

		program.Use();
		trans.Rotate(Vector3d(0.f, 1.f, 0.f), 20 * Time::DeltaTime());
		trans.Rotate(Vector3d(1.f, 0.f, 0.f), 20 * Time::DeltaTime());
		program.SetUniform(program.GetUniform("model"), trans.GetLocalToWorldMatrix());
		program.SetUniform(program.GetUniform("view"), camera.GetWorldToCameraMatrix());
		program.SetUniform(program.GetUniform("projection"), camera.GetProjectionMatrix());
		program.SetUniform(program.GetUniform("lightPos"), Vector3d(1.0f + std::sin(Time::ElapsedTime()) * 2.0f, sin(Time::ElapsedTime() / 2.0f) * 1.0f, 0.f));
		program.SetUniform(program.GetUniform("viewPos"), camera.GetTransform().GetPosition());
		program.SetUniform(program.GetUniform("objectColor"), Color::Lerp(Color::purple, Color::yellow, Math::PingPong(Time::ElapsedTime() / 2.f, 1.f)));
		Graphics::DrawArrays(vao, Triangles, 0, vtxCount);
		program.UnUse();

		platform.SwapBuffer();
	}

	return 0;
}