#include "MeshRenderer.h"
#include "Bounds.h"
#include "BoxCollider.h"
#include "Camera.h"
#include "Debug.h"
#include "FrameBuffer.h"
#include "GameObject.h"
#include "Graphics.h"
#include "Light.h"
#include "Material.h"
#include "Math.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Transform.h"
#include "Utility.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "VertexBuffer.h"

namespace Theodore {
  MeshRenderer::MeshRenderer() : Renderer("MeshRenderer"), mMaterial(nullptr), mMesh(nullptr) {
    mPrimitive = Primitive::Triangles;
  }

  MeshRenderer::~MeshRenderer() {
    SafeDealloc(mMaterial);
    if (mMesh && !mMesh->IsManaged()) {
      SafeDealloc(mMesh);
    }
  }

  void MeshRenderer::SetMaterial(Material* mat) {
    mMaterial = mat;
    mProgram = mMaterial->shader;
  }

  void MeshRenderer::SetMesh(Mesh* mesh) {
    if (mesh == NULL) {
      return;
    }

    mMesh = mesh;

    Buffer* buffer = new Buffer(BufferType::BufferVertex);
    VertexSemantic semantic = mMesh->GetVertexSemantic();
    size_t size = 0;

    buffer->Data(nullptr,
                 mMesh->GetVertexCount() * sizeof(Vector3d) +
                     mMesh->GetUvCount() * sizeof(Vector2d) +
                     mMesh->GetNormalCount() * sizeof(Vector3d),
                 mMesh->GetBufferUsage());
    if (semantic & VertexSemantic::SemanticPosition) {
      buffer->SubData(&mMesh->mVertices.front(), size, mMesh->GetVertexCount() * sizeof(Vector3d));
      size += mMesh->GetVertexCount() * sizeof(Vector3d);
    }
    if (semantic & VertexSemantic::SemanticTexCoord) {
      buffer->SubData(&mMesh->mUvs.front(), size, mMesh->GetUvCount() * sizeof(Vector2d));
      size += mMesh->GetUvCount() * sizeof(Vector2d);
    }
    if (semantic & VertexSemantic::SemanticNormal) {
      buffer->SubData(&mMesh->mNormals.front(), size, mMesh->GetNormalCount() * sizeof(Vector3d));
      size += mMesh->GetNormalCount() * sizeof(Vector3d);
    }
    mVbos.push_back(buffer);

    if (semantic & VertexSemantic::SemanticFaces) {
      Buffer* index = new Buffer(BufferType::BufferIndex);
      size_t indexSize = 0;
      switch (mMesh->GetIndexFormat()) {
      case IndexFormat::UInt16:
        indexSize = sizeof(unsigned short);
        break;
      case IndexFormat::UInt32:
        indexSize = sizeof(unsigned int);
        break;
      }

      index->Data(&mMesh->mFaces.front(), mMesh->GetFaceCount() * indexSize,
                  mMesh->GetBufferUsage());
      mEbos.push_back(index);
      mVao->BindElements(*mEbos.front());
    }

    size_t offset = 0;
    if (semantic & VertexSemantic::SemanticPosition) {
      mVao->BindAttribute(mProgram->GetAttribute("position"), *mVbos.front(), 3, sizeof(Vector3d),
                          offset);
      offset += mMesh->GetVertexCount() * sizeof(Vector3d);
    }
    if (semantic & VertexSemantic::SemanticTexCoord) {
      mVao->BindAttribute(mProgram->GetAttribute("uvs"), *mVbos.front(), 2, sizeof(Vector2d),
                          offset);
      offset += mMesh->GetUvCount() * sizeof(Vector2d);
    }
    if (semantic & VertexSemantic::SemanticNormal) {
      mVao->BindAttribute(mProgram->GetAttribute("normal"), *mVbos.front(), 3, sizeof(Vector3d),
                          offset);
      offset += mMesh->GetNormalCount() * sizeof(Vector3d);
    }

    mMesh->RecalculateBounds();
    mBounds.SetMinMax(mMesh->GetBounds()->GetMin(), mMesh->GetBounds()->GetMax());
  }

  Mesh* MeshRenderer::GetMesh() const { return mMesh; }

  void MeshRenderer::Update(float deltaTime) {
    // See Christer Ericson's Real-time Collision Detection, p. 87, or
    // James Arvo's "Transforming Axis-aligned Bounding Boxes" in Graphics Gems 1, pp. 548-550.
    Matrix4x4 world = mTransform->GetLocalToWorldMatrix();
    Matrix4x4 model = mTransform->GetWorldToLocalMatrix();

    Vector3d center = mMesh->GetBounds()->GetCenter();
    Vector3d extents = mMesh->GetBounds()->GetExtents();
    Vector3d powScale = Math::Pow(Matrix4x4::DecomposeScale(world), 2.f);

    Vector3d newCenter = Matrix4x4::DecomposeTranslation(world) +
                         powScale * Vector3d(Math::Dot(Vector3d(model.rows[0]), center),
                                             Math::Dot(Vector3d(model.rows[1]), center),
                                             Math::Dot(Vector3d(model.rows[2]), center));

    Vector3d newExtents = powScale * Vector3d(Math::AbsDot(Vector3d(model.rows[0]), extents),
                                              Math::AbsDot(Vector3d(model.rows[1]), extents),
                                              Math::AbsDot(Vector3d(model.rows[2]), extents));

    mBounds.SetMinMax(newCenter - newExtents, newCenter + newExtents);
  }

  void MeshRenderer::Render() {
    Graphics::DrawCube(mBounds.GetCenter(), mBounds.GetSize(), Color::red);

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

    mProgram->Use();
    mProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
    mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
    mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
    mProgram->SetUniform("viewPos", SceneManager::GetMainCamera()->GetTransform()->GetPosition());

    unsigned int spotLightCount = 0;
    unsigned int pointLightCount = 0;

    if (mMaterial->renderTexture) {
      mProgram->SetUniform("material.renderTexture", 0);
      mProgram->SetUniform("material.isRenderTexture", true);
      Graphics::BindTexture(0, mMaterial->renderTexture->GetTexture(Attachment::Color0));
    } else {
      mProgram->SetUniform("material.isRenderTexture", false);
      mProgram->SetUniform("material.ambient", mMaterial->ambient);
      mProgram->SetUniform("material.diffuse", mMaterial->diffuse);
      mProgram->SetUniform("material.specular", mMaterial->specular);
      mProgram->SetUniform("material.shininess", mMaterial->shininess);

      if (mMaterial->texture0) {
        mProgram->SetUniform("material.texture0", 0);
        mProgram->SetUniform("material.isTexture0", true);
        Graphics::BindTexture(0, mMaterial->texture0);
      } else {
        mProgram->SetUniform("material.isTexture0", false);
      }

      if (mMaterial->texture1) {
        mProgram->SetUniform("material.texture1", 1);
        mProgram->SetUniform("material.isTexture1", true);
        Graphics::BindTexture(1, mMaterial->texture1);
      } else {
        mProgram->SetUniform("material.isTexture1", false);
      }

      // Global directional light
      mProgram->SetUniform("dirLight.direction",
                           SceneManager::GetGlobalLight()->GetTransform()->GetPosition().Negate());
      mProgram->SetUniform("dirLight.ambient", SceneManager::GetGlobalLight()->ambient);
      mProgram->SetUniform("dirLight.diffuse", SceneManager::GetGlobalLight()->diffuse);
      mProgram->SetUniform("dirLight.specular", SceneManager::GetGlobalLight()->specular);

      std::vector<Light*> lights = mGameObject->GetAllLights();
      for (unsigned int i = 0; i < lights.size(); i++) {
        switch (lights[i]->type) {
        case LightType::SpotLight:
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].position",
                               lights[i]->GetTransform()->GetPosition());
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].direction",
                               lights[i]->GetTransform()->GetForward());
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].cutOff",
                               lights[i]->cutOff);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].outerCutOff",
                               lights[i]->outerCutOff);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].constant",
                               lights[i]->constant);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].linear",
                               lights[i]->linear);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].quadratic",
                               lights[i]->quadratic);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].ambient",
                               lights[i]->ambient);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].diffuse",
                               lights[i]->diffuse);
          mProgram->SetUniform("spotLights[" + std::to_string(spotLightCount) + "].specular",
                               lights[i]->specular);
          if (Light::MaxLightCount > spotLightCount) {
            spotLightCount++;
          }
          break;
        case LightType::PointLight:
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].position",
                               lights[i]->GetTransform()->GetPosition());
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].constant",
                               lights[i]->constant);
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].linear",
                               lights[i]->linear);
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].quadratic",
                               lights[i]->quadratic);
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].ambient",
                               lights[i]->ambient);
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].diffuse",
                               lights[i]->diffuse);
          mProgram->SetUniform("pointLights[" + std::to_string(pointLightCount) + "].specular",
                               lights[i]->specular);
          if (Light::MaxLightCount > spotLightCount) {
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
      mProgram->SetUniform("lightSpaceMatrix", Matrix4x4::Identity());
    }

    mProgram->SetUniform("spotLightCount", spotLightCount);
    mProgram->SetUniform("pointLightCount", pointLightCount);

    if (mMesh) {
      if (mMesh->mSemantic & VertexSemantic::SemanticFaces) {
        Graphics::DrawElements(*mVao, mPrimitive, 0, mMesh->GetFaceCount(),
                               mMesh->GetIndexFormat());
      } else {
        Graphics::DrawArrays(*mVao, mPrimitive, 0, mMesh->GetVertexCount());
      }
    }

    Graphics::BindTexture(0, NULL);
    Graphics::BindTexture(1, NULL);
    mProgram->UnUse();

    if (mIsVisibleGizmos) {
      mNormalVisualizeProgram->Use();
      mNormalVisualizeProgram->SetUniform("model", mTransform->GetLocalToWorldMatrix());
      mNormalVisualizeProgram->SetUniform("view",
                                          SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
      mNormalVisualizeProgram->SetUniform("projection",
                                          SceneManager::GetMainCamera()->GetProjectionMatrix());
      if (mMesh) {
        if (mMesh->GetFaceCount() > 0) {
          Graphics::DrawElements(*mVao, mPrimitive, 0, mMesh->GetFaceCount(),
                                 mMesh->GetIndexFormat());
        } else {
          Graphics::DrawArrays(*mVao, mPrimitive, 0, mMesh->GetVertexCount());
        }
      }
      mNormalVisualizeProgram->UnUse();
    }
  }
}