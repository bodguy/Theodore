#include "MeshRenderer.h"
#include "Material.h"
#include "Shader.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Light.h"
#include "Mesh.h"
#include "Graphics.h"
#include "Utility.h"

namespace Quark {
	MeshRenderer::MeshRenderer() : Renderer("MeshRenderer"), mMaterial(nullptr), mMesh(nullptr) {
		mPrimitive = Primitive::Triangles;
	}

	MeshRenderer::~MeshRenderer() {
		SafeDealloc(mMaterial);
		SafeDealloc(mMesh);
	}

	void MeshRenderer::SetMaterial(Material* mat) {
		mMaterial = mat;
		mProgram = mMaterial->GetProgram();
	}

	void MeshRenderer::SetMesh(Mesh* mesh) {
		mMesh = mesh;

		Buffer* buffer = new Buffer(BufferType::BufferVertex);
		buffer->Data(nullptr, mMesh->GetVertexCount() * sizeof(Vector3d) + mMesh->GetNormalCount() * sizeof(Vector3d), BufferUsage::StaticDraw);
		buffer->SubData(&mMesh->mVertices.front(), 0, mMesh->GetVertexCount() * sizeof(Vector3d));
		//buffer->SubData(&mMesh->mUvs.front(), mMesh->GetVertexCount() * sizeof(Vector3d), mMesh->GetUvCount() * sizeof(Vector2d));
		buffer->SubData(&mMesh->mNormals.front(), mMesh->GetVertexCount() * sizeof(Vector3d), mMesh->GetNormalCount() * sizeof(Vector3d));
		mVbos.push_back(buffer);

		//Buffer* index = new Buffer(BufferType::BufferIndex);
		//index->Data(&mMesh->mFaces.front(), sizeof(unsigned int) * mMesh->GetFaceCount(), BufferUsage::StaticDraw);
		//mEbos.push_back(index);

		mVao->BindAttribute(mProgram->GetAttribute("position"), *mVbos.front(), 3, sizeof(Vector3d), 0);
		mVao->BindAttribute(mProgram->GetAttribute("normal"), *mVbos.front(), 3, sizeof(Vector3d), mMesh->GetVertexCount() * sizeof(Vector3d));
		//mVao->BindElements(*mEbos.front());
	}

	void MeshRenderer::Update(double deltaTime) {
	}

	void MeshRenderer::Render() {
		if (mMesh && mMaterial && mProgram) {
			mProgram->Use();
			mProgram->SetUniform("model", mGameObject->GetTransform()->GetLocalToWorldMatrix());
			// to make uniform buffer
			mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			mProgram->SetUniform("cameraPosition", SceneManager::GetMainCamera()->GetTransform()->GetPosition());
			
			mProgram->SetUniform("material.ambient", mMaterial->mAmbient);
			mProgram->SetUniform("material.diffuse", mMaterial->mDiffuse);
			mProgram->SetUniform("material.specular", mMaterial->mSpecular);
			mProgram->SetUniform("material.shininess", mMaterial->mShininess);
	
			mProgram->SetUniform("light.position", SceneManager::GetGlobalLight()->GetTransform()->GetPosition());
			mProgram->SetUniform("light.ambient", SceneManager::GetGlobalLight()->mAmbient);
			mProgram->SetUniform("light.diffuse", SceneManager::GetGlobalLight()->mDiffuse);
			mProgram->SetUniform("light.specular", SceneManager::GetGlobalLight()->mSpecular);

			if (mMesh->GetFaceCount() > 0) {
				Graphics::DrawElements(*mVao, mPrimitive, 0, mMesh->GetFaceCount(), mMesh->GetIndexFormat());
			} else {
				Graphics::DrawArrays(*mVao, mPrimitive, 0, mMesh->GetVertexCount());
			}
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