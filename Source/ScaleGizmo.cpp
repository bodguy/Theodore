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

	void ScaleGizmo::Render() {
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
		Graphics::DrawArrays(mVao, Enumeration::Lines, 0, 6);
		mProgram.UnUse();
	}
}