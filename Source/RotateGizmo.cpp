#include "RotateGizmo.h"
#include "Vector3d.h"
#include "Math.h"
#include "File.h"
#include "Graphics.h"
#include "Time.h"
#include "Quaternion.h"

namespace Quark {
	RotateGizmo::RotateGizmo() : Gizmo(), mDegree(5), mHalfCircleSegmentCount(64) {
		InputStream verts;
		for(int axis = 0; axis < 3; axis++) {
			float angleStart = 0;
			for (int i = 0; i < mHalfCircleSegmentCount; i ++) {
				float ng = angleStart + Math::pi * ((float)i / (float)mHalfCircleSegmentCount);
				Vector3d axisPos = Vector3d(Math::Cos(ng), Math::Sin(ng), 0.f);
				verts.Vec3(Vector3d(axisPos[axis], axisPos[(axis+1)%3], axisPos[(axis+2)%3]));
			}
		}

		Buffer buffer(Enumeration::BufferVertex);
		buffer.Data(verts.Pointer(), verts.Size(), Enumeration::StaticDraw);
		mVao.BindAttribute(mProgram.GetAttribute("position"), buffer, 3, sizeof(Vector3d), 0);
	}

	RotateGizmo::~RotateGizmo() {
	}

	void RotateGizmo::Render() {
		// x axis
		mTransform.Rotate(Vector3d(1.f, 0.f, 0.f), Math::Radians(90.f));
		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
		mProgram.SetUniform(mProgram.GetUniform("color"), Color::GizmoRed);
		Graphics::DrawArrays(mVao, Enumeration::LineStrip, 0, mHalfCircleSegmentCount);
		mProgram.UnUse();

		/*
		mTransform.Rotate(Vector3d::backward, Math::Radians(90.f));
		Quaternion originalRot = mTransform.mRotation;
		mTransform.mRotation = originalRot * Quaternion::AngleAxis(Math::Radians(180), Vector3d::backward);
		*/

		mProgram.Use();
		mProgram.SetUniform(mProgram.GetUniform("model"), mTransform.GetLocalToWorldMatrix());
		mProgram.SetUniform(mProgram.GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
		mProgram.SetUniform(mProgram.GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
		mProgram.SetUniform(mProgram.GetUniform("color"), Color::GizmoGreen);
		Graphics::DrawArrays(mVao, Enumeration::LineStrip, 0, mHalfCircleSegmentCount);
		mProgram.UnUse();
	}
}