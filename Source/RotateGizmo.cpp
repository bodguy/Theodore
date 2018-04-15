#include "RotateGizmo.h"
#include "Vector3d.h"
#include "Math.h"
#include "File.h"
#include "Graphics.h"

namespace Quark {
	RotateGizmo::RotateGizmo() : Gizmo(), mDegree(5) {
		InputStream verts;
		for (int i = 0; i <= Math::degrees; i += mDegree) {
			// x axis
			verts.Vec3(Vector3d(Math::Cos(Math::Radians(i)), 0.f, Math::Sin(Math::Radians(i))));
			verts.Vec3(Vector3d(Math::Cos(Math::Radians(i + mDegree)), 0.f, Math::Sin(Math::Radians(i + mDegree))));
			// y axis
			verts.Vec3(Vector3d(Math::Cos(Math::Radians(i)), Math::Sin(Math::Radians(i)), 0.f));
			verts.Vec3(Vector3d(Math::Cos(Math::Radians(i + mDegree)), Math::Sin(Math::Radians(i + mDegree)), 0.f));
			// z axis
			verts.Vec3(Vector3d(0.f, Math::Cos(Math::Radians(i)), Math::Sin(Math::Radians(i))));
			verts.Vec3(Vector3d(0.f, Math::Cos(Math::Radians(i + mDegree)), Math::Sin(Math::Radians(i + mDegree))));
		}

		InputStream colour;
		for (int i = 0; i <= Math::degrees; i += mDegree) {
			colour.Color4(Color::GizmoGreen);
			colour.Color4(Color::GizmoGreen);
			colour.Color4(Color::GizmoRed);
			colour.Color4(Color::GizmoRed);
			colour.Color4(Color::GizmoBlue);
			colour.Color4(Color::GizmoBlue);
		}

		Buffer buffer1(Enumeration::BufferVertex);
		buffer1.Data(verts.Pointer(), verts.Size(), Enumeration::StaticDraw);
		Buffer buffer2(Enumeration::BufferVertex);
		buffer2.Data(colour.Pointer(), colour.Size(), Enumeration::StaticDraw);

		mVao.BindAttribute(mProgram.GetAttribute("position"), buffer1, 3, sizeof(Vector3d), 0);
		mVao.BindAttribute(mProgram.GetAttribute("color"), buffer2, 4, sizeof(Color), 0);
	}

	RotateGizmo::~RotateGizmo() {
	}

	void RotateGizmo::Render(const Camera& cam) {
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), cam.GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), cam.GetProjectionMatrix());
		Graphics::DrawArrays(mVao, Enumeration::Lines, 0, Math::degrees / mDegree * 6);
		mProgram.UnUse();
	}
}