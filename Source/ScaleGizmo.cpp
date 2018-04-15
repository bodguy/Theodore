#include "ScaleGizmo.h"
#include "Vector3d.h"
#include "Math.h"
#include "File.h"
#include "Graphics.h"

namespace Quark {
	ScaleGizmo::ScaleGizmo() : Gizmo() {

	}

	ScaleGizmo::~ScaleGizmo() {
	}

	void ScaleGizmo::Render(const Camera& cam) {
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		Graphics::DrawArrays(mVao, Enumeration::Lines, 0, 6);
		mProgram.UnUse();
	}
}