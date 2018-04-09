#include "QuarkEngine.h"

Platform platform;
Input input;
Time timer;

void Update() {
	//Debug::Log(timer.FPS());
	if (Input::GetKeyDown(KEY_ESCAPE)) {
		platform.Quit();
	}
}

void Render() {
	Graphics::Clear(Color(0.2f, 0.2f, 0.4f, 0.5f), ColorBits | DepthBits);

	platform.SwapBuffer();
}

int main() {
	platform.Initialize("Verlet Integration", 800, 600);
	platform.SetVSync(true);
	Graphics::Enable(DepthTest);
	Graphics::SetPolygonMode(FillMode::WireFrame);

	while (platform.IsRunning()) {
		platform.Update();
		timer.Update();
		input.Update();
		Update();
		Render();
	}
	return 0;
}