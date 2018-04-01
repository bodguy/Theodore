#include "QuarkEngine.h"

int main(int argc, char** argv) {
	Platform platform;
	Time time;
	platform.Initialize("Modern OpenGL #1", 800, 600, false, 4, 1);
	platform.SetVSync(false);

	while (platform.IsRunning()) {
		time.Update();

		Graphics::Clear(Color(0.129f, 0.586f, 0.949f, 1.0f), ColorBits);

		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex2f(-0.5f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex2f(0.f, 0.5f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex2f(0.5f, 0.f);
		glEnd();

		Debug::Log("%d\n", time.FPS());

		platform.SwapBuffer();
		platform.Update();
	}

	return 0;
}