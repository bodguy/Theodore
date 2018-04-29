#include "Gizmo.h"
#include "File.h"
#include "Utility.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "Math.h"
#include "Time.h"

namespace Quark { 
	Gizmo::Gizmo(Enumeration::GizmoType type) :Component("Gizmo"), mVao(nullptr), mTransform(), mProgram(nullptr), mType(type) {
		Shader* vs = AssetManager::RequestShader("Shaders/gizmo/vs.glsl", Enumeration::VertexShader);
		Shader* fs = AssetManager::RequestShader("Shaders/gizmo/fs.glsl", Enumeration::FragmentShader);
		mProgram = new Program(*vs, *fs);

		InputStream verts;
		Buffer buffer(Enumeration::BufferVertex);
		mVao = new VertexArray();

		switch (mType) {
			case Enumeration::TranslationGizmo:
				verts.Vec3(Vector3d(0.f, 0.f, 0.f));
				verts.Vec3(Vector3d(1.f, 0.f, 0.f));
				verts.Vec3(Vector3d(0.f, 0.f, 0.f));
				verts.Vec3(Vector3d(0.f, 1.f, 0.f));
				verts.Vec3(Vector3d(0.f, 0.f, 0.f));
				verts.Vec3(Vector3d(0.f, 0.f, 1.f));
				mTransform.Rotate(Vector3d(0.f, 1.f, 0.f), 45.f);
				break;
			case Enumeration::ScaleGizmo:
				break;
			case Enumeration::RotationGizmo:
			{
				int mHalfCircleSegmentCount = 64;
				for (int axis = 0; axis < 3; axis++) {
					float angleStart = 0;
					for (int i = 0; i < mHalfCircleSegmentCount; i++) {
						float ng = angleStart + Math::pi * ((float)i / (float)mHalfCircleSegmentCount);
						Vector3d axisPos = Vector3d(Math::Cos(ng), Math::Sin(ng), 0.f);
						verts.Vec3(Vector3d(axisPos[axis], axisPos[(axis + 1) % 3], axisPos[(axis + 2) % 3]));
					}
				}
				break;
			}
		}
		buffer.Data(verts.Pointer(), verts.Size(), Enumeration::StaticDraw);
		mVao->BindAttribute(mProgram->GetAttribute("position"), buffer, 3, sizeof(Vector3d), 0);
	}

	Gizmo::~Gizmo() {
		SafeDealloc(mVao);
		SafeDealloc(mProgram);
	}

	Transform& Gizmo::GetTransform() {
		return mTransform;
	}

	void Gizmo::Update(double deltaTime) {
		UnUsed(deltaTime);
	}

	void Gizmo::Render() {
		glDepthRange(0, 0.01);
		switch (mType) {
		case Enumeration::TranslationGizmo:
			mProgram->Use();
			mProgram->SetUniform(mProgram->GetUniform("model"), mTransform.GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoRed);
			Graphics::DrawArrays(*mVao, Enumeration::Lines, 0, 2);
			mProgram->UnUse();

			mProgram->Use();
			mProgram->SetUniform(mProgram->GetUniform("model"), mTransform.GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoGreen);
			Graphics::DrawArrays(*mVao, Enumeration::Lines, 2, 2);
			mProgram->UnUse();

			mProgram->Use();
			mProgram->SetUniform(mProgram->GetUniform("model"), mTransform.GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoBlue);
			Graphics::DrawArrays(*mVao, Enumeration::Lines, 4, 2);
			mProgram->UnUse();
			break;
		case Enumeration::ScaleGizmo:
			break;
		case Enumeration::RotationGizmo:
			// rotation X
			mProgram->Use();
			Quaternion quat = Quaternion::AngleAxis(Math::Radians(90.f), Vector3d(0.f, 1.f, 0.f));
			quat *= Quaternion::AngleAxis(Math::Radians(-90.f), Vector3d(1.f, 0.f, 0.f));
			mTransform.SetRotation(quat);
			mProgram->SetUniform(mProgram->GetUniform("model"), mTransform.GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoRed);
			Graphics::DrawArrays(*mVao, Enumeration::LineStrip, 0, 64);
			mProgram->UnUse();

			// rotation Z
			mProgram->Use();
			mTransform.SetRotation(Quaternion::identity);
			mProgram->SetUniform(mProgram->GetUniform("model"), mTransform.GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoBlue);
			Graphics::DrawArrays(*mVao, Enumeration::LineStrip, 0, 64);
			mProgram->UnUse();

			// rotation Y
			mProgram->Use();
			mTransform.SetRotation(Quaternion::AngleAxis(Math::Radians(-90.f), Vector3d(1.f, 0.f, 0.f)));
			mProgram->SetUniform(mProgram->GetUniform("model"), mTransform.GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoGreen);
			Graphics::DrawArrays(*mVao, Enumeration::LineStrip, 0, 64);
			mProgram->UnUse();
			break;
		}
		glDepthRange(0, 1.0);
	}

	bool Gizmo::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool Gizmo::Destroy() {
		return false;
	}
}