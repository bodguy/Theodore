#include "QuarkEngine.h"

int main() {
	Platform platform;
	Input input;
	Time timer;
	platform.Initialize("Color Transform", 200, 200);
	platform.SetVSync(true);

	float scale = 1.f;
	while (platform.IsRunning()) {
		platform.Update();
		timer.Update();
		input.Update();

		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		} else if (Input::GetKeyDown(KEY_1)) {
			scale++;
			Time::SetTimeScale(scale);
			Debug::Log(scale);
		} else if (Input::GetKeyDown(KEY_2)) {
			scale--;
			Time::SetTimeScale(scale);
			Debug::Log(scale);
		}
		Debug::Log(Time::FPS());

		Color c = Color::Lerp(Color(1.f, 0.f, 1.f, 1.f), Color(0.f, 0.f, 1.f, 1.f), Math::PingPong(Time::ElapsedTime() / 2, 1.f));
		Graphics::Clear(c, ColorBits);
		platform.SwapBuffer();
	}

	return 0;
}