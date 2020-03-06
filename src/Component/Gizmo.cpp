// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Gizmo.h"
#include "Asset/AssetManager.h"
#include "Asset/Pipeline.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Graphics/VertexArray.h"
#include "Helper/File.h"
#include "Helper/Utility.h"
#include "Math/Mathf.h"
#include "Core/GameObject.h"
#include "Core/SceneManager.h"
#include "Transform.h"

namespace Theodore {
  Gizmo::Gizmo(GizmoType type) : Component("Gizmo"), vertexArray(nullptr), pipeline(nullptr), gizmoType(type) {
		pipeline = Shader::Find("Gizmo");

    InputStream verts;
    GPUBuffer buffer(BufferType::BufferVertex);
		vertexArray = new VertexArray();

    switch (gizmoType) {
      case GizmoType::Translation:
        verts.Vec3(Vector3d(0.f, 0.f, 0.f));
        verts.Vec3(Vector3d(1.f, 0.f, 0.f));
        verts.Vec3(Vector3d(0.f, 0.f, 0.f));
        verts.Vec3(Vector3d(0.f, 1.f, 0.f));
        verts.Vec3(Vector3d(0.f, 0.f, 0.f));
        verts.Vec3(Vector3d(0.f, 0.f, 1.f));
        // mTransform->Rotate(Vector3d(0.f, 1.f, 0.f), 45.f);
        break;
      case GizmoType::Scale:
        break;
      case GizmoType::Rotation: {
        int mHalfCircleSegmentCount = 64;
        for (int axis = 0; axis < 3; axis++) {
          for (int i = 0; i < mHalfCircleSegmentCount; i++) {
            float ng = Mathf::pi * ((float)i / (float)mHalfCircleSegmentCount);
            Vector3d axisPos = Vector3d(Mathf::Cos(ng), Mathf::Sin(ng), 0.f);
            verts.Vec3(Vector3d(axisPos[axis], axisPos[(axis + 1) % 3], axisPos[(axis + 2) % 3]));
          }
        }
        break;
      }
    }
    buffer.Data(verts.Pointer(), verts.Size(), BufferUsage::StaticDraw);
    vertexArray->BindAttribute(pipeline->GetAttribute("position"), buffer, 3, sizeof(Vector3d), 0);
  }

  Gizmo::~Gizmo() { SafeDealloc(vertexArray); }

  void Gizmo::Update(float deltaTime) { UnUsed(deltaTime); }

  void Gizmo::Render() {
    glDepthRange(0, 0.01);
    switch (gizmoType) {
      case GizmoType::Translation:
        pipeline->Use();
        pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
        pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
        pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
        pipeline->SetUniform(pipeline->GetUniform("color"), Color::GizmoRed);
        Graphics::DrawArrays(*vertexArray, Primitive::Lines, 0, 2);
        pipeline->UnUse();

        pipeline->Use();
        pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
        pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
        pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
        pipeline->SetUniform(pipeline->GetUniform("color"), Color::GizmoGreen);
        Graphics::DrawArrays(*vertexArray, Primitive::Lines, 2, 2);
        pipeline->UnUse();

        pipeline->Use();
        pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
        pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
        pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
        pipeline->SetUniform(pipeline->GetUniform("color"), Color::GizmoBlue);
        Graphics::DrawArrays(*vertexArray, Primitive::Lines, 4, 2);
        pipeline->UnUse();
        break;
      case GizmoType::Scale:
        break;
      case GizmoType::Rotation:
        // rotation X
        pipeline->Use();
        Quaternion quat = Quaternion::AngleAxis(Mathf::Radians(90.f), Vector3d(0.f, 1.f, 0.f));
        quat *= Quaternion::AngleAxis(Mathf::Radians(-90.f), Vector3d(1.f, 0.f, 0.f));
        transform->SetRotation(quat);
        pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
        pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
        pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
        pipeline->SetUniform(pipeline->GetUniform("color"), Color::GizmoRed);
        Graphics::DrawArrays(*vertexArray, Primitive::LineStrip, 0, 64);
        pipeline->UnUse();

        // rotation Z
        pipeline->Use();
        transform->SetRotation(Quaternion::identity);
        pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
        pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
        pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
        pipeline->SetUniform(pipeline->GetUniform("color"), Color::GizmoBlue);
        Graphics::DrawArrays(*vertexArray, Primitive::LineStrip, 0, 64);
        pipeline->UnUse();

        // rotation Y
        pipeline->Use();
        transform->SetRotation(Quaternion::AngleAxis(Mathf::Radians(-90.f), Vector3d(1.f, 0.f, 0.f)));
        pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
        pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
        pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
        pipeline->SetUniform(pipeline->GetUniform("color"), Color::GizmoGreen);
        Graphics::DrawArrays(*vertexArray, Primitive::LineStrip, 0, 64);
        pipeline->UnUse();
        break;
    }
    glDepthRange(0, 1.0);
  }

  bool Gizmo::CompareEquality(const Object& rhs) const { return false; }

  bool Gizmo::Destroy() { return false; }
}  // namespace Theodore