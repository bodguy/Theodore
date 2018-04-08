#include "QuarkEngine.h"

int main() {
	Platform platform;
	Input input;
	Time time;

	platform.Initialize("Verlet Integration", 800, 400);
	platform.SetVSync(true);
	Graphics::Enable(DepthTest);
	Graphics::SetPolygonMode(FillMode::WireFrame);

	Cloth cloth(10, 10);

	while (platform.IsRunning()) {
		platform.Update();
		time.Update();
		input.Update();

		Debug::Log(time.FPS());

		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}

		cloth.WindForce(Vector3d(0.05, 0, 0.05) * 0.5f * 0.5f);
		cloth.Update(0.5f * 0.5f);
		Graphics::Clear(Color(0.2f, 0.2f, 0.4f, 0.5f), ColorBits | DepthBits);
		cloth.Render();
		platform.SwapBuffer();
	}
	return 0;
}