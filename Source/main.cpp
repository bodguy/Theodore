#include "QuarkEngine.h"

bool IsComment(const std::string& c) {
	return (c.empty()) || (c.at(0) == '#');
}

bool IsGeometryData(const std::string& c) {
	return (c.at(0) == 'v');
}

bool IsFaceData(const std::string& c) {
	return (c.at(0) == 'f');
}

int main() {
	Platform platform;
	Time time;
	Input input;

	platform.Initialize("Modern OpenGL 01 - Basic Triangle", 800, 600);
	platform.SetVSync(false);
	Graphics::SetPolygonMode(FillMode::WireFrame);

	Shader vs(VertexShader);
	Shader tc(TessControlShader);
	Shader te(TessEvalShader);
	Shader gs(GeometryShader);
	Shader fs(FragmentShader);

	File shaderSource;
	shaderSource.Open("Core/Shaders/VertexShader.glsl", Read);
	vs.Compile(shaderSource.ReadUntilEnd());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/TessControlShader.glsl", Read);
	tc.Compile(shaderSource.ReadUntilEnd());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/TessEvalShader.glsl", Read);
	te.Compile(shaderSource.ReadUntilEnd());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/GeometryShader.glsl", Read);
	gs.Compile(shaderSource.ReadUntilEnd());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/FragmentShader.glsl", Read);
	fs.Compile(shaderSource.ReadUntilEnd());
	shaderSource.Close();

	Program program(vs, fs, gs, tc, te);

	InputStream vertexData;
	InputStream indexData;
	File fmtFile;
	fmtFile.Open("Core/Contents/Icosahedron.fmt", OpenMode::Read);
	if (fmtFile.IsOpen()) {
		while (fmtFile.Validate()) {
			std::string s = fmtFile.GetLine();

			if (!IsComment(s)) {
				if (IsGeometryData(s)) {
					float x, y, z;
					fmtFile.Read("%f %f %f\n", &x, &y, &z);
					vertexData.Vec3(Vector3d(x, y, z));
				} else if (IsFaceData(s)) {
					int f0, f1, f2;
					fmtFile.Read("%d %d %d\n", &f0, &f1, &f2);
					indexData.Int32(f0); indexData.Int32(f1); indexData.Int32(f2);
				}
			}
		}
		fmtFile.Close();
	}

	Buffer vertexBuffer(vertexData.Pointer(), vertexData.Size(), StaticDraw, BufferType::BufferVertex);
	Buffer indexBuffer(indexData.Pointer(), indexData.Size(), BufferUsage::StaticDraw, BufferType::BufferIndex);

	VertexArray vao;
	vao.BindAttribute(program.GetAttribute("Position"), vertexBuffer, 3, 3 * sizeof(float), NULL);
	vao.BindElements(indexBuffer);

	Transform transfrom;
	transfrom.SetScale(Vector3d(0.5f, 0.5f, 0.5f));
	float TessLevelInner = 1;
    float TessLevelOuter = 1;

	while (platform.IsRunning()) {
		time.Update();
		input.Update();

		Graphics::Clear(Color(0.0, 0.0, 0.0, 1.0), ColorBits | DepthBits);

		if(Input::GetKeyDown(KEY_ESCAPE)) {
			platform.Quit();
		}

		if(Input::GetKeyDown(KEY_UP)) {
			TessLevelInner++;
			TessLevelOuter++;
		} else if(Input::GetKeyDown(KEY_DOWN)) {
			TessLevelInner--;
			TessLevelOuter--;
		}

		transfrom.Rotate(Vector3d(1.f, 0.f, 0.f), Math::Radians(50 * time.ElapsedTime()));

		program.Use();
		program.SetUniform(program.GetUniform("Modelview"), transfrom.GetLocalToWorldMatrix());
		program.SetUniform(program.GetUniform("TessLevelInner"), TessLevelInner);
		program.SetUniform(program.GetUniform("TessLevelOuter"), TessLevelOuter);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		Graphics::DrawElements(vao, Primitive::Patches, 0, indexData.Size() / sizeof(int), GL_UNSIGNED_INT);

		platform.SwapBuffer();
		platform.Update();
	}

	return 0;
}