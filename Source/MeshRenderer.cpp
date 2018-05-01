#include "MeshRenderer.h"
#include "Material.h"
#include "Shader.h"
#include "AssetManager.h"
#include "Enumeration.h"
#include "Graphics.h"
#include "Camera.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Utility.h"

namespace Quark {
	MeshRenderer::MeshRenderer() : Component("MeshRenderer"), mMaterial(nullptr), mMeshCache(nullptr), mProgram(nullptr) {
		Shader* vs = AssetManager::RequestShader("Shaders/light/vs.glsl", Enumeration::VertexShader);
		Shader* fs = AssetManager::RequestShader("Shaders/light/fs.glsl", Enumeration::FragmentShader);
		mProgram = new Program();
		mProgram->AttachShader(*vs);
		mProgram->AttachShader(*fs);
		mProgram->Link();

		mMeshCache = mGameObject->GetComponent<MeshFilter>()->GetMesh();
		mMeshCache->GetVao()->BindAttribute(mProgram->GetAttribute("position"), *mMeshCache->GetVbo(), 3, sizeof(Vector3d), 0);
		mMeshCache->GetVao()->BindAttribute(mProgram->GetAttribute("normal"), *mMeshCache->GetVbo(), 3, sizeof(Vector3d), sizeof(Vector3d));
	}

	MeshRenderer::~MeshRenderer() {
		SafeDealloc(mMaterial);
		SafeDealloc(mProgram);
	}

	void MeshRenderer::SetMaterial(Material* mat) {
		mMaterial = mat;
	}

	void MeshRenderer::Update(double deltaTime) {
	}

	void MeshRenderer::Render() {
		if (mMeshCache) {
			mProgram->Use();
			mProgram->SetUniform(mProgram->GetUniform("model"), mGameObject->GetTransform()->GetLocalToWorldMatrix());
			mProgram->SetUniform(mProgram->GetUniform("view"), Camera::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform(mProgram->GetUniform("projection"), Camera::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform(mProgram->GetUniform("viewPos"), Camera::GetMainCamera()->GetTransform().GetPosition());
			mProgram->SetUniform(mProgram->GetUniform("lightPos"), Vector3d(0.f, 10.f, 0.f));
			mProgram->SetUniform(mProgram->GetUniform("objectColor"), mMaterial->GetAmbient());
			Graphics::DrawArrays(*mMeshCache->GetVao(), Enumeration::Triangles, 0, mMeshCache->GetVertexCount());
			mProgram->UnUse();
		}
	}

	bool MeshRenderer::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool MeshRenderer::Destroy() {
		return false;
	}
}