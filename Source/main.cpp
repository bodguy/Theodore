#include "QuarkEngine.h"

const int Faces[] = {
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,
        11, 6,  7,
        11, 7,  8,
        11, 8,  9,
        11, 9,  10,
        11, 10, 6,
        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,
        2,  7, 6,
        3,  8, 7,
        4,  9, 8,
        5, 10, 9,
        1, 6, 10 };

const float Verts[] = {
    0.000f,  0.000f,  1.000f,
    0.894f,  0.000f,  0.447f,
    0.276f,  0.851f,  0.447f,
    -0.724f,  0.526f,  0.447f,
    -0.724f, -0.526f,  0.447f,
    0.276f, -0.851f,  0.447f,
    0.724f,  0.526f, -0.447f,
    -0.276f,  0.851f, -0.447f,
    -0.894f,  0.000f, -0.447f,
    -0.276f, -0.851f, -0.447f,
    0.724f, -0.526f, -0.447f,
    0.000f,  0.000f, -1.000f };

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
	vs.Compile(shaderSource.ReadUntilEnd().c_str());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/TessControlShader.glsl", Read);
	tc.Compile(shaderSource.ReadUntilEnd().c_str());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/TessEvalShader.glsl", Read);
	te.Compile(shaderSource.ReadUntilEnd().c_str());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/GeometryShader.glsl", Read);
	gs.Compile(shaderSource.ReadUntilEnd().c_str());
	shaderSource.Close();

	shaderSource.Open("Core/Shaders/FragmentShader.glsl", Read);
	fs.Compile(shaderSource.ReadUntilEnd().c_str());
	shaderSource.Close();

	Program program(vs, fs, gs, tc, te);

	VertexDataBuffer vertexData;
	int stride = 3;
	int vtxCount = sizeof(Verts) / sizeof(Verts[0]) / stride; // 12
	for(int i = 0; i < vtxCount; i++) {
		vertexData.Vec3(Vector3d(Verts[i * stride], Verts[i * stride + 1], Verts[i * stride + 2]));
	}

	VertexDataBuffer indexData;
	int idxCount = sizeof(Faces) / sizeof(Faces[0]); // 20
	for(int i = 0; i < idxCount; i++) {
		indexData.Int32(Faces[i]);
	}

	Buffer vertexBuffer(vertexData.Pointer(), vertexData.Size(), StaticDraw, BufferType::BufferVertex);
	Buffer indexBuffer(indexData.Pointer(), indexData.Size(), BufferUsage::StaticDraw, BufferType::BufferIndex);

	VertexArray vao;
	vao.BindAttribute(program.GetAttribute("Position"), vertexBuffer, 3, stride * sizeof(float), NULL);
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
		Graphics::DrawElements(vao, Primitive::Patches, 0, idxCount, GL_UNSIGNED_INT);

		platform.SwapBuffer();
		platform.Update();
	}

	return 0;
}