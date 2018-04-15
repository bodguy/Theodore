#include "QuarkEngine.h"

int main() {
	Platform platform;
	Input input;
	Time timer;
	AssetManager manager;
	Camera camera;
	platform.Initialize("Lighting", 800, 600);
	platform.SetVSync(true);
	Graphics::Enable(DepthTest);
	Graphics::SetPolygonMode(WireFrame);

	TranslateGizmo gizmo1;
	RotateGizmo gizmo2;
	
	camera.GetTransform().SetPosition(Vector3d(0.1254f, 0.2907f, 1.6509f));
	while (platform.IsRunning()) {
		platform.Update();
		timer.Update();
		input.Update();
		
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}
		else if (Input::GetKeyHeld(KEY_D)) {
			camera.GetTransform().Translate(Vector3d(0.5f * Time::DeltaTime(), 0.f, 0.f));
		}
		else if (Input::GetKeyHeld(KEY_A)) {
			camera.GetTransform().Translate(Vector3d(-0.5f * Time::DeltaTime(), 0.f, 0.f));
		}
		else if (Input::GetKeyHeld(KEY_W)) {
			camera.GetTransform().Translate(Vector3d(0.f, 0.5f * Time::DeltaTime(), 0.f));
		}
		else if (Input::GetKeyHeld(KEY_S)) {
			camera.GetTransform().Translate(Vector3d(0.f, -0.5f * Time::DeltaTime(), 0.f));
		}
		else if (Input::GetKeyHeld(KEY_Q)) {
			camera.GetTransform().Translate(Vector3d(0.f, 0.f, -0.5f * Time::DeltaTime()));
		}
		else if (Input::GetKeyHeld(KEY_E)) {
			camera.GetTransform().Translate(Vector3d(0.f, 0.f, 0.5f * Time::DeltaTime()));
		}

		Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), ColorBits | DepthBits);
		gizmo1.Render(camera);
		gizmo2.Render(camera);
		platform.SwapBuffer();
	}

	return 0;
}