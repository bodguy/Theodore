// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "MeshRenderer.h"

#include "Asset/Texture2D.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Graphics.h"
#include "Graphics/VertexBuffer.h"
#include "Helper/Debug.h"
#include "Helper/Utility.h"
#include "Math/Math.h"
#include "Material.h"
#include "Asset/Mesh.h"
#include "Object/GameObject.h"
#include "Object/SceneManager.h"
#include "Transform.h"

namespace Theodore {
  MeshRenderer::MeshRenderer() : Renderer("MeshRenderer"), material(nullptr), mesh(nullptr) { primitive = Primitive::Triangles; }

  MeshRenderer::~MeshRenderer() {
    SafeDealloc(material);
    if (mesh && !mesh->IsManaged()) {
      SafeDealloc(mesh);
    }
  }

  void MeshRenderer::SetMaterial(Material* mat) {
		material = mat;
		pipeline = material->GetShader();
  }

  void MeshRenderer::SetMesh(Mesh* mesh) {
    if (mesh == NULL) {
      return;
    }

		mesh = mesh;

    Buffer* buffer = new Buffer(BufferType::BufferVertex);
    VertexSemantic semantic = mesh->GetVertexSemantic();
    size_t size = 0;

    buffer->Data(nullptr, mesh->GetVertexCount() * sizeof(Vector3d) + mesh->GetUvCount() * sizeof(Vector2d) + mesh->GetNormalCount() * sizeof(Vector3d), mesh->GetBufferUsage());
    if (semantic & VertexSemantic::SemanticPosition) {
      buffer->SubData(&mesh->vertices.front(), size, mesh->GetVertexCount() * sizeof(Vector3d));
      size += mesh->GetVertexCount() * sizeof(Vector3d);
    }
    if (semantic & VertexSemantic::SemanticTexCoord) {
      buffer->SubData(&mesh->mUvs.front(), size, mesh->GetUvCount() * sizeof(Vector2d));
      size += mesh->GetUvCount() * sizeof(Vector2d);
    }
    if (semantic & VertexSemantic::SemanticNormal) {
      buffer->SubData(&mesh->mNormals.front(), size, mesh->GetNormalCount() * sizeof(Vector3d));
      size += mesh->GetNormalCount() * sizeof(Vector3d);
    }
    vertexBuffers.push_back(buffer);

    if (semantic & VertexSemantic::SemanticFaces) {
      Buffer* index = new Buffer(BufferType::BufferIndex);
      size_t indexSize = 0;
      switch (mesh->GetIndexFormat()) {
        case IndexFormat::UInt16:
          indexSize = sizeof(unsigned short);
          break;
        case IndexFormat::UInt32:
          indexSize = sizeof(unsigned int);
          break;
      }

      index->Data(&mesh->triangles.front(), mesh->GetTriangleCount() * indexSize, mesh->GetBufferUsage());
      indexBuffers.push_back(index);
      vertexArray->BindElements(*indexBuffers.front());
    }

    size_t offset = 0;
    if (semantic & VertexSemantic::SemanticPosition) {
      vertexArray->BindAttribute(pipeline->GetAttribute("position"), *vertexBuffers.front(), 3, sizeof(Vector3d), offset);
      offset += mesh->GetVertexCount() * sizeof(Vector3d);
    }
    if (semantic & VertexSemantic::SemanticTexCoord) {
      vertexArray->BindAttribute(pipeline->GetAttribute("uvs"), *vertexBuffers.front(), 2, sizeof(Vector2d), offset);
      offset += mesh->GetUvCount() * sizeof(Vector2d);
    }
    if (semantic & VertexSemantic::SemanticNormal) {
      vertexArray->BindAttribute(pipeline->GetAttribute("normal"), *vertexBuffers.front(), 3, sizeof(Vector3d), offset);
      offset += mesh->GetNormalCount() * sizeof(Vector3d);
    }

    mesh->RecalculateBounds();
    bounds.SetMinMax(mesh->GetBounds()->GetMin(), mesh->GetBounds()->GetMax());
  }

  Mesh* MeshRenderer::GetMesh() const { return mesh; }

  void MeshRenderer::Update(float deltaTime) {
    // See Christer Ericson's Real-time Collision Detection, p. 87, or
    // James Arvo's "Transforming Axis-aligned Bounding Boxes" in Graphics Gems 1, pp. 548-550.
    Matrix4x4 world = transform->GetLocalToWorldMatrix();
    Matrix4x4 model = transform->GetWorldToLocalMatrix();

    Vector3d center = mesh->GetBounds()->GetCenter();
    Vector3d extents = mesh->GetBounds()->GetExtents();
    Vector3d powScale = Math::Pow(Matrix4x4::DecomposeScale(world), 2.f);

    Vector3d newCenter = Matrix4x4::DecomposeTranslation(world) +
                         powScale * Vector3d(Math::Dot(Vector3d(model.rows[0]), center), Math::Dot(Vector3d(model.rows[1]), center), Math::Dot(Vector3d(model.rows[2]), center));

    Vector3d newExtents = powScale * Vector3d(Math::AbsDot(Vector3d(model.rows[0]), extents), Math::AbsDot(Vector3d(model.rows[1]), extents), Math::AbsDot(Vector3d(model.rows[2]), extents));

    bounds.SetMinMax(newCenter - newExtents, newCenter + newExtents);
  }

  void MeshRenderer::Render() {
    Graphics::DrawCube(bounds.GetCenter(), bounds.GetSize(), Color::red);

    /*
    std::vector<Camera*>& cameras = this->mGameObject->GetAllCameras();
    for (Camera* cam : cameras) {
      FrameBuffer* frameBuffer = cam->GetRenderTexture();
      if (frameBuffer) {
        Graphics::BindFrameBuffer(frameBuffer);
        Graphics::Clear(BufferBits::ColorBits);
        InternalRender();
      }
    }

    Graphics::BindFrameBuffer(NULL);
    */
    InternalRender();
  }

  bool MeshRenderer::CompareEquality(const Object& rhs) const { return false; }

  bool MeshRenderer::Destroy() { return false; }

  void MeshRenderer::InternalRender() {
    Graphics::Disable(Capabilities::Blending);
    Graphics::Disable(Capabilities::AlphaTest);

    pipeline->Use();
    pipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
    pipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
    pipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    pipeline->SetUniform("viewPos", SceneManager::GetMainCamera()->GetTransform()->GetPosition());

    unsigned int spotLightCount = 0;
    unsigned int pointLightCount = 0;

    if (material->GetRenderTexture()) {
      pipeline->SetUniform("material.renderTexture", 0);
      pipeline->SetUniform("material.isRenderTexture", true);
      Graphics::BindTexture(0, material->GetRenderTexture()->GetTexture(Attachment::Color0));
    } else {
      pipeline->SetUniform("material.isRenderTexture", false);
      pipeline->SetUniform("material.ambient", material->GetAmbient());
      pipeline->SetUniform("material.diffuse", material->GetDiffuse());
      pipeline->SetUniform("material.specular", material->GetSpecular());
      pipeline->SetUniform("material.shininess", material->GetShininess());

      if (material->GetTexture0()) {
        pipeline->SetUniform("material.texture0", 0);
        pipeline->SetUniform("material.isTexture0", true);
        Graphics::BindTexture(0, material->GetTexture0());
      } else {
        pipeline->SetUniform("material.isTexture0", false);
      }

      if (material->GetTexture1()) {
        pipeline->SetUniform("material.texture1", 1);
        pipeline->SetUniform("material.isTexture1", true);
        Graphics::BindTexture(1, material->GetTexture1());
      } else {
        pipeline->SetUniform("material.isTexture1", false);
      }

      // Global directional light
      pipeline->SetUniform("dirLight.direction", SceneManager::GetGlobalLight()->GetTransform()->GetPosition().Negate());
      pipeline->SetUniform("dirLight.ambient", SceneManager::GetGlobalLight()->ambient);
      pipeline->SetUniform("dirLight.diffuse", SceneManager::GetGlobalLight()->diffuse);
      pipeline->SetUniform("dirLight.specular", SceneManager::GetGlobalLight()->specular);

      std::vector<Light*> lights = gameObject->GetAllLights();
      for (unsigned int i = 0; i < lights.size(); i++) {
        switch (lights[i]->type) {
          case LightType::SpotLight:
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].position", lights[i]->GetTransform()->GetPosition());
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].direction", lights[i]->GetTransform()->GetForward());
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].cutOff", lights[i]->cutOff);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].outerCutOff", lights[i]->outerCutOff);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].constant", lights[i]->constant);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].linear", lights[i]->linear);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].quadratic", lights[i]->quadratic);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].ambient", lights[i]->ambient);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].diffuse", lights[i]->diffuse);
            pipeline->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].specular", lights[i]->specular);
            if (Light::maxLightCount > spotLightCount) {
              spotLightCount++;
            }
            break;
          case LightType::PointLight:
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].position", lights[i]->GetTransform()->GetPosition());
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].constant", lights[i]->constant);
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].linear", lights[i]->linear);
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].quadratic", lights[i]->quadratic);
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].ambient", lights[i]->ambient);
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].diffuse", lights[i]->diffuse);
            pipeline->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].specular", lights[i]->specular);
            if (Light::maxLightCount > spotLightCount) {
              pointLightCount++;
            }
            break;
          default:
            break;
        }
      }

      // Matrix4x4 proj = Matrix4x4::Orthogonal(-10.f, 10.f, -10.f, 10.f,
      // SceneManager::GetMainCamera()->GetNearClipPlane(),
      // SceneManager::GetMainCamera()->GetFarClipPlane());
      // Matrix4x4 view =
      // Matrix4x4::LookAt(SceneManager::GetMainCamera()->GetTransform()->GetPosition(),
      // Vector3d::zero, Vector3d::up);
      // Matrix4x4 viewProj = proj * view;

      // shadowProgram->Use();
      // shadowProgram->SetUniform("lightSpaceMatrix", viewProj);
      // shadowProgram->SetUniform("model", mGameObject->GetTransform()->GetLocalToWorldMatrix());
      pipeline->SetUniform("lightSpaceMatrix", Matrix4x4::Identity());
    }

    pipeline->SetUniform("spotLightCount", spotLightCount);
    pipeline->SetUniform("pointLightCount", pointLightCount);

    if (mesh) {
      if (mesh->vertexSemantic & VertexSemantic::SemanticFaces) {
        Graphics::DrawElements(*vertexArray, primitive, 0, mesh->GetTriangleCount(), mesh->GetIndexFormat());
      } else {
        Graphics::DrawArrays(*vertexArray, primitive, 0, mesh->GetVertexCount());
      }
    }

    Graphics::BindTexture(0, NULL);
    Graphics::BindTexture(1, NULL);
    pipeline->UnUse();

    if (isVisibleGizmos) {
      normalVisualizePipeline->Use();
      normalVisualizePipeline->SetUniform("model", transform->GetLocalToWorldMatrix());
      normalVisualizePipeline->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
      normalVisualizePipeline->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
      if (mesh) {
        if (mesh->GetTriangleCount() > 0) {
          Graphics::DrawElements(*vertexArray, primitive, 0, mesh->GetTriangleCount(), mesh->GetIndexFormat());
        } else {
          Graphics::DrawArrays(*vertexArray, primitive, 0, mesh->GetVertexCount());
        }
      }
      normalVisualizePipeline->UnUse();
    }
  }
}  // namespace Theodore