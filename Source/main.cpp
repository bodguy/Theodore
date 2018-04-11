#include "QuarkEngine.h"

int main() {
	Platform platform;
	Input input;
	Time time;

	platform.Initialize("Verlet Integration", 800, 600, false, 4, 3);
	platform.SetVSync(true);

	Shader vs(VertexShader), fs(FragmentShader);
	File src;
	src.Open("Core/Shaders/vs.glsl", Read);
	if(src.IsOpen()) {
		vs.Compile(src.ReadUntilEnd());
	}
	src.Close();
	src.Open("Core/Shaders/fs.glsl", Read);
	if(src.IsOpen()) {
		fs.Compile(src.ReadUntilEnd());
	}
	src.Close();

	Program program;
	program.AttachShader(vs);
	program.AttachShader(fs);
	program.Link();

	InputStream stream;
	stream.Vec2(Vector2d(-0.5f, -0.5f));
	stream.Vec2(Vector2d(0.f, 0.5f));
	stream.Vec2(Vector2d(0.5f, -0.5f));

	Buffer vertex(BufferVertex);
	vertex.Data(stream.Pointer(), stream.Size(), StaticDraw);
	VertexArray vao;
	vao.BindAttribute(program.GetAttribute("position"), vertex, 2, sizeof(Vector2d), 0);
	
	while (platform.IsRunning()) {
		platform.Update();
		time.Update();
		input.Update();

		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}

		Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), ColorBits);
		program.Use();
		Graphics::DrawArrays(vao, LineLoop, 0, 3);
		platform.SwapBuffer();
	}

	return 0;
}