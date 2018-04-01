#include "Cubemap.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "File.h"
#include "Vector3d.h"
#include "Graphics.h"
#include "AssetManager.h"
#include "Utility.h"
#include "Matrix4x4.h"
#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Quark {
	float CubeVertices[] = {
		-1.0f, 1.0f, -1.0f ,-1.0f, -1.0f, -1.0f ,1.0f, -1.0f, -1.0f ,1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
	};

	Cubemap::Cubemap(std::list<std::string>& filelist, Enumeration::TextureFormat format) {
		mVertexData = new VertexDataBuffer();
		for (int i = 0; i < sizeof(CubeVertices) / sizeof(float) / 3; i++) {
			mVertexData->Vec3(Vector3d(CubeVertices[i*3], CubeVertices[i*3+1], CubeVertices[i*3+2]));
		}

		mVertexBuffer = new Buffer(mVertexData->Pointer(), mVertexData->Size(), Enumeration::StaticDraw, Enumeration::BufferVertex);
		mVertexArray = new VertexArray();
		mVertexArray->BindAttribute(0, *mVertexBuffer, 3, 3 * sizeof(float), 0);

		Shader vs(Enumeration::VertexShader), fs(Enumeration::FragmentShader);

		File shaderSource("Shaders/cube_vs.glsl", Enumeration::Read);
		vs.Compile(shaderSource.ReadUntilEnd());
		shaderSource.Close();

		shaderSource.Open("Shaders/cube_fs.glsl", Enumeration::Read);
		fs.Compile(shaderSource.ReadUntilEnd());
		shaderSource.Close();

		mCubeProgram = new Program(vs, fs);

		Graphics::Enable(Enumeration::CubemapSeamless);
		glGenTextures(1, &mCubemapID);

		std::list<std::string>::const_iterator iter = filelist.begin();
		for (int i = Enumeration::PositiveX; i <= Enumeration::NegativeZ; i++) {
			AssetManager::RequestTexture(mCubemapID, *iter++, format, static_cast<Enumeration::CubemapFace>(i));
		}
	}

	Cubemap::~Cubemap() {
		SafeDealloc(mVertexData);
		SafeDealloc(mVertexBuffer);
		SafeDealloc(mVertexArray);
		SafeDealloc(mCubeProgram);
		glDeleteTextures(1, &mCubemapID);
	}

	void Cubemap::Render() {
		mCubeProgram->Use();
		glActiveTexture(GL_TEXTURE0);
		glDepthFunc(GL_LEQUAL);
		mCubeProgram->SetUniform(mCubeProgram->GetUniform("cubemap"), (unsigned int)0);
		glBindTexture(Enumeration::Cube, mCubemapID);
		Graphics::DrawArrays(*mVertexArray, Enumeration::Triangles, 0, sizeof(CubeVertices) / sizeof(float) / 3);
		glBindTexture(Enumeration::Cube, NULL);
		glDepthFunc(GL_LESS); 
	}

	void Cubemap::SetMatrix(const Matrix4x4& view, const Matrix4x4& proj) {
		mCubeProgram->SetUniform(mCubeProgram->GetUniform("view"), Matrix4x4::ToMatrix3x3(view));
		mCubeProgram->SetUniform(mCubeProgram->GetUniform("proj"), proj);
	}
}
