#include "QuarkEngine.h"

int main() {
	Platform platform;
	Input input;
	Time timer;
	platform.Initialize("Verlet Integration", 800, 600);
	platform.SetVSync(true);

	Shader vs(VertexShader), fs(FragmentShader), cs(ComputeShader);
	File src;
	src.Open("Core/Shaders/vs.glsl", Read);
	if(src.IsOpen()) {
		vs.Compile(src.ReadUntilEnd());
		src.Close();
	}
	
	src.Open("Core/Shaders/fs.glsl", Read);
	if(src.IsOpen()) {
		fs.Compile(src.ReadUntilEnd());
		src.Close();
	}

	src.Open("Core/Shaders/compute.glsl", Read);
	if (src.IsOpen()) {
		cs.Compile(src.ReadUntilEnd());
		src.Close();
	}

	Program program, program2;
	program.AttachShader(vs);
	program.AttachShader(fs);
	program.Link();

	program2.AttachShader(cs);
	program2.Link();

	InputStream stream;
	stream.Vec2(Vector2d(-0.5f, -0.5f));
	stream.Vec2(Vector2d(0.f, 0.5f));
	stream.Vec2(Vector2d(0.5f, -0.5f));

	Buffer vertex(BufferStorage);
	vertex.Data(stream.Pointer(), stream.Size(), DynamicDraw);
	VertexArray vao;
	vao.BindAttribute(program.GetAttribute("position"), vertex, 2, sizeof(Vector2d), 0);

	while (platform.IsRunning()) {
		platform.Update();
		timer.Update();
		input.Update();

		if (Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}

		Graphics::Clear(Color(0.f, 0.f, 0.f, 1.f), ColorBits);
		
		program2.Use();
		vertex.BindRange(0, 0, 3 * sizeof(Vector2d));
		program2.DispatchCompute(1, 1, 1);
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
		program2.UnUse();

		program.Use();
		Graphics::DrawArrays(vao, LineLoop, 0, 3);
		program.UnUse();
		platform.SwapBuffer();
	}

	return 0;
}