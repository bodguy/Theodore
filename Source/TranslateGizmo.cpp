#include "TranslateGizmo.h"
#include "Vector3d.h"
#include "Math.h"
#include "File.h"
#include "Graphics.h"

namespace Quark {
	TranslateGizmo::TranslateGizmo() : Gizmo() {
		InputStream verts;
		verts.Vec3(Vector3d(0.f, 0.f, 0.f));
		verts.Vec3(Vector3d(1.f, 0.f, 0.f));
		verts.Vec3(Vector3d(0.f, 0.f, 0.f));
		verts.Vec3(Vector3d(0.f, 1.f, 0.f));
		verts.Vec3(Vector3d(0.f, 0.f, 0.f));
		verts.Vec3(Vector3d(0.f, 0.f, 1.f));

		Buffer buffer(Enumeration::BufferVertex);
		buffer.Data(verts.Pointer(), verts.Size(), Enumeration::StaticDraw);
		mVao.BindAttribute(mProgram.GetAttribute("position"), buffer, 3, sizeof(Vector3d), 0);
	}

	TranslateGizmo::~TranslateGizmo() {

	}

	void TranslateGizmo::Render(const Camera& cam) {
		mTransform.Rotate(Vector3d(0.f, 1.f, 0.f), Math::Radians(45));
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		mProgram.SetUniform(mProgram.GetUniform("color"), Color::GizmoRed);
		Graphics::DrawArrays(mVao, Enumeration::Lines, 0, 2);
		mProgram.UnUse();

		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		mProgram.SetUniform(mProgram.GetUniform("color"), Color::GizmoGreen);
		Graphics::DrawArrays(mVao, Enumeration::Lines, 2, 2);
		mProgram.UnUse();

		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		mProgram.SetUniform(mProgram.GetUniform("color"), Color::GizmoBlue);
		Graphics::DrawArrays(mVao, Enumeration::Lines, 4, 2);
		mProgram.UnUse();
	}
}