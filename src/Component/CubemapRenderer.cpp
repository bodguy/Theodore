// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "CubemapRenderer.h"

#include "Asset/Shader.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Math/Mathf.h"
#include "Asset/Mesh.h"
#include "Helper/ShapeGenerator.h"
#include "Object/SceneManager.h"

namespace Theodore {
  CubemapRenderer::CubemapRenderer() : MeshRenderer() {
		pipeline = Shader::Find("Cubemap");
    SetMesh(ShapeGenerator::GenerateCube());
    Graphics::Enable(Capabilities::CubemapSeamless);
    glGenTextures(1, &textureId);
  }

  CubemapRenderer::~CubemapRenderer() { glDeleteTextures(1, &textureId); }

  unsigned int CubemapRenderer::GetTextureID() const { return textureId; }

  void CubemapRenderer::Update(float deltaTime) {}

  void CubemapRenderer::Render() {
    pipeline->Use();
    glDepthFunc(GL_LEQUAL);

    Camera* cam = SceneManager::GetMainCamera();
    pipeline->SetUniform("view", Matrix4x4::ToMatrix3x3(cam->GetWorldToCameraMatrix()));
    pipeline->SetUniform("projection", Matrix4x4::Perspective(Mathf::Radians(cam->GetFieldOfView()), cam->GetAspectRatio(), cam->GetNearClipPlane(), cam->GetFarClipPlane()));

    pipeline->SetUniform("cubemap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(static_cast<GLenum>(TextureDimension::CubeMap), textureId);
    Graphics::DrawArrays(*vertexArray, primitive, 0, mesh->GetVertexCount());
    Graphics::BindTexture(0, NULL);

    glDepthFunc(GL_LESS);
    pipeline->UnUse();
  }

  bool CubemapRenderer::CompareEquality(const Object& rhs) const { return false; }

  bool CubemapRenderer::Destroy() { return false; }
}  // namespace Theodore