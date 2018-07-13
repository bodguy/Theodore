#include "Gizmo.h"
#include "File.h"
#include "Utility.h"
#include "AssetManager.h"
#include "Graphics.h"
#include "Math.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "SceneManager.h"

namespace Theodore { 
	Gizmo::Gizmo(GizmoType type) :Component("Gizmo"), mVao(nullptr), mProgram(nullptr), mType(type) {
		mProgram = Shader::Find("Gizmo");

		InputStream verts;
		Buffer buffer(BufferType::BufferVertex);
		mVao = new VertexArray();

		switch (mType) {
			case GizmoType::Translation:
				verts.Vec3(Vector3d(0.f, 0.f, 0.f));
				verts.Vec3(Vector3d(1.f, 0.f, 0.f));
				verts.Vec3(Vector3d(0.f, 0.f, 0.f));
				verts.Vec3(Vector3d(0.f, 1.f, 0.f));
				verts.Vec3(Vector3d(0.f, 0.f, 0.f));
				verts.Vec3(Vector3d(0.f, 0.f, 1.f));
				//mTransform->Rotate(Vector3d(0.f, 1.f, 0.f), 45.f);
				break;
			case GizmoType::Scale:
				break;
			case GizmoType::Rotation:
			{
				int mHalfCircleSegmentCount = 64;
				for (int axis = 0; axis < 3; axis++) {
					for (int i = 0; i < mHalfCircleSegmentCount; i++) {
						float ng = Math::pi * ((float)i / (float)mHalfCircleSegmentCount);
						Vector3d axisPos = Vector3d(Math::Cos(ng), Math::Sin(ng), 0.f);
						verts.Vec3(Vector3d(axisPos[axis], axisPos[(axis + 1) % 3], axisPos[(axis + 2) % 3]));
					}
				}
				break;
			}
		}
		buffer.Data(verts.Pointer(), verts.Size(), BufferUsage::StaticDraw);
		mVao->BindAttribute(mProgram->GetAttribute("position"), buffer, 3, sizeof(Vector3d), 0);
	}

	Gizmo::~Gizmo() {
		SafeDealloc(mVao);
	}

	void Gizmo::Update(double deltaTime) {
		UnUsed(deltaTime);
	}

	void Gizmo::Render() {
		glDepthRange(0, 0.01);
		switch (mType) {
		case GizmoType::Translation:
			mProgram->Use();
			mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoRed);
			Graphics::DrawArrays(*mVao, Primitive::Lines, 0, 2);
			mProgram->UnUse();

			mProgram->Use();
			mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoGreen);
			Graphics::DrawArrays(*mVao, Primitive::Lines, 2, 2);
			mProgram->UnUse();

			mProgram->Use();
			mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoBlue);
			Graphics::DrawArrays(*mVao, Primitive::Lines, 4, 2);
			mProgram->UnUse();
			break;
		case GizmoType::Scale:
			break;
		case GizmoType::Rotation:
			// rotation X
			mProgram->Use();
			Quaternion quat = Quaternion::AngleAxis(Math::Radians(90.f), Vector3d(0.f, 1.f, 0.f));
			quat *= Quaternion::AngleAxis(Math::Radians(-90.f), Vector3d(1.f, 0.f, 0.f));
			mTransform->SetRotation(quat);
			mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoRed);
			Graphics::DrawArrays(*mVao, Primitive::LineStrip, 0, 64);
			mProgram->UnUse();

			// rotation Z
			mProgram->Use();
			mTransform->SetRotation(Quaternion::identity);
			mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoBlue);
			Graphics::DrawArrays(*mVao, Primitive::LineStrip, 0, 64);
			mProgram->UnUse();

			// rotation Y
			mProgram->Use();
			mTransform->SetRotation(Quaternion::AngleAxis(Math::Radians(-90.f), Vector3d(1.f, 0.f, 0.f)));
			mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("color"), Color::GizmoGreen);
			Graphics::DrawArrays(*mVao, Primitive::LineStrip, 0, 64);
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