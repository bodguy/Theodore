#include "MeshRenderer.h"
#include "Material.h"
#include "Shader.h"
#include "AssetManager.h"
#include "Enumeration.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "Utility.h"
#include "Transform.h"
#include "Camera.h"

namespace Quark {
	MeshRenderer::MeshRenderer() : Renderer("MeshRenderer"), mMaterial(nullptr), mMesh(nullptr) {
		mMesh = mGameObject->GetComponent<MeshFilter>()->GetMesh();
	}

	MeshRenderer::~MeshRenderer() {
		SafeDealloc(mMaterial);
	}

	void MeshRenderer::SetMaterial(Material* mat) {
		mMaterial = mat;
		//mMesh->GetVao()->BindAttribute(mProgram->GetAttribute("position"), *mMesh->GetVbo(), 3, sizeof(Vector3d), 0);
		//mMeshCache->GetVao()->BindAttribute(mProgram->GetAttribute("normal"), *mMeshCache->GetVbo(), 3, sizeof(Vector3d), sizeof(Vector3d));
	}

	void MeshRenderer::Update(double deltaTime) {
	}

	void MeshRenderer::Render() {
		//if (mMesh) {
		//	mProgram->Use();
		//	mProgram->SetUniform(mProgram->GetUniform("model"), mGameObject->GetTransform()->GetLocalToWorldMatrix());
		//	mProgram->SetUniform(mProgram->GetUniform("view"), SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
		//	mProgram->SetUniform(mProgram->GetUniform("projection"), SceneManager::GetMainCamera()->GetProjectionMatrix());
		//	mProgram->SetUniform(mProgram->GetUniform("viewPos"), SceneManager::GetMainCamera()->GetTransform()->GetPosition());
		//	mProgram->SetUniform(mProgram->GetUniform("lightPos"), Vector3d(0.f, 10.f, 0.f));
		//	mProgram->SetUniform(mProgram->GetUniform("objectColor"), mMaterial->GetAmbient());
		//	Graphics::DrawArrays(*mMesh->GetVao(), Enumeration::LineStrip, 0, mMesh->GetVertexCount());
		//	mProgram->UnUse();
		//}
	}

	bool MeshRenderer::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool MeshRenderer::Destroy() {
		return false;
	}
}