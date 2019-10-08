// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "CubemapRenderer.h"
#include "Asset/Shader.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Math/Math.h"
#include "Object/Component/sub/Mesh.h"
#include "Object/Component/sub/ShapeGenerator.h"
#include "Object/SceneManager.h"

namespace Theodore {
  CubemapRenderer::CubemapRenderer() : MeshRenderer() {
    mProgram = Shader::Find("Cubemap");
    SetMesh(ShapeGenerator::GenerateCube());
    Graphics::Enable(Capabilities::CubemapSeamless);
    glGenTextures(1, &mTextureID);
  }

  CubemapRenderer::~CubemapRenderer() { glDeleteTextures(1, &mTextureID); }

  unsigned int CubemapRenderer::GetTextureID() const { return mTextureID; }

  void CubemapRenderer::Update(float deltaTime) {}

  void CubemapRenderer::Render() {
    mProgram->Use();
    glDepthFunc(GL_LEQUAL);

    Camera* cam = SceneManager::GetMainCamera();
    mProgram->SetUniform("view", Matrix4x4::ToMatrix3x3(cam->GetWorldToCameraMatrix()));
    mProgram->SetUniform("projection", Matrix4x4::Perspective(Math::Radians(cam->GetFieldOfView()), cam->GetAspectRatio(), cam->GetNearClipPlane(), cam->GetFarClipPlane()));

    mProgram->SetUniform("cubemap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(static_cast<GLenum>(TextureDimension::CubeMap), mTextureID);
    Graphics::DrawArrays(*mVao, mPrimitive, 0, mMesh->GetVertexCount());
    Graphics::BindTexture(0, NULL);

    glDepthFunc(GL_LESS);
    mProgram->UnUse();
  }

  bool CubemapRenderer::CompareEquality(const Object& rhs) const { return false; }

  bool CubemapRenderer::Destroy() { return false; }
}  // namespace Theodore