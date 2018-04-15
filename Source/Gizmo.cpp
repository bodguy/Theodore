#include "Gizmo.h"
#include "File.h"

namespace Quark { 
	Gizmo::Gizmo() : mVao(), mTransform(), mProgram() {
		using namespace Enumeration;
		Shader vs(VertexShader), fs(FragmentShader);
		File source;
		source.Open("Core/Shaders/gizmo/vs.glsl", Read);
		if (source.IsOpen()) {
			vs.Compile(source.ReadUntilEnd());
			source.Close();
		}
		source.Open("Core/Shaders/gizmo/fs.glsl", Read);
		if (source.IsOpen()) {
			fs.Compile(source.ReadUntilEnd());
			source.Close();
		}
		mProgram.AttachShader(vs);
		mProgram.AttachShader(fs);
		mProgram.Link();
	}

	Gizmo::~Gizmo() {
	}

	Transform& Gizmo::GetTransform() {
		return mTransform;
	}
}