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

		InputStream colour;
		colour.Color4(Color::GizmoRed);
		colour.Color4(Color::GizmoRed);
		colour.Color4(Color::GizmoGreen);
		colour.Color4(Color::GizmoGreen);
		colour.Color4(Color::GizmoBlue);
		colour.Color4(Color::GizmoBlue);

		Buffer buffer1(Enumeration::BufferVertex);
		buffer1.Data(verts.Pointer(), verts.Size(), Enumeration::StaticDraw);
		Buffer buffer2(Enumeration::BufferVertex);
		buffer2.Data(colour.Pointer(), colour.Size(), Enumeration::StaticDraw);

		mVao.BindAttribute(mProgram.GetAttribute("position"), buffer1, 3, sizeof(Vector3d), 0);
		mVao.BindAttribute(mProgram.GetAttribute("color"), buffer2, 4, sizeof(Color), 0);
	}

	TranslateGizmo::~TranslateGizmo() {

	}

	void TranslateGizmo::Render(const Camera& cam) {
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		Graphics::DrawArrays(mVao, Enumeration::Lines, 0, 6);
		mProgram.UnUse();
	}
}