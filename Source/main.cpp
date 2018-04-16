#include "QuarkEngine.h"

int main() {
	Platform platform;
	Input input;
	Time timer;
	AssetManager manager;
	Camera camera;
	platform.Initialize("Lighting", 800, 600, false, 4, 3, 16);
	platform.SetVSync(true);
	Graphics::Enable(DepthTest);

	TranslateGizmo gizmo1;
	float distanceZ = 0.f;
	Vector3d distanceFromCamera = Vector3d(camera.GetTransform().GetPosition().x, 
										camera.GetTransform().GetPosition().y,
										camera.GetTransform().GetPosition().z - distanceZ);
	Plane plane(Vector3d::forward, distanceFromCamera);

	float speed = 0.7f;
	while (platform.IsRunning()) {
		platform.Update();
		timer.Update();
		input.Update();
		
		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}
		else if (Input::GetKeyHeld(KEY_D)) {
			camera.GetTransform().Translate(Vector3d(speed * Time::DeltaTime(), 0.f, 0.f));
		}
		else if (Input::GetKeyHeld(KEY_A)) {
			camera.GetTransform().Translate(Vector3d(-speed * Time::DeltaTime(), 0.f, 0.f));
		}
		else if (Input::GetKeyHeld(KEY_W)) {
			camera.GetTransform().Translate(Vector3d(0.f, speed * Time::DeltaTime(), 0.f));
		}
		else if (Input::GetKeyHeld(KEY_S)) {
			camera.GetTransform().Translate(Vector3d(0.f, -speed * Time::DeltaTime(), 0.f));
		}
		else if (Input::GetKeyHeld(KEY_Q)) {
			camera.GetTransform().Translate(Vector3d(0.f, 0.f, -speed * Time::DeltaTime()));
		}
		else if (Input::GetKeyHeld(KEY_E)) {
			camera.GetTransform().Translate(Vector3d(0.f, 0.f, speed * Time::DeltaTime()));
		}

		if (Input::GetMouseButtonHeld(MOUSE_LEFT)) {
			Ray ray = camera.ScreenPointToRay(Input::GetMousePosition());
			float enter = 0.f;

			Debug::Log(ray);
			if (plane.Raycast(ray, &enter)) {
				Vector3d hitPoint = ray.GetPoint(enter);
				gizmo1.GetTransform().SetPosition(hitPoint);
			}
		}

		Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), ColorBits | DepthBits);
		gizmo1.Render(camera);
		platform.SwapBuffer();
	}

	return 0;
}