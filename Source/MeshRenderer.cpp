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
#include "Math.h"
#include "Utility.h"

namespace Quark {
	MeshRenderer::MeshRenderer() : Renderer("MeshRenderer"), mMaterial(nullptr), mMesh(nullptr) {
		mPrimitive = Primitive::Triangles;
#ifdef _DEBUG
		DEBUG_PROGRAM = Shader::Find("DebugNormal");
#endif
	}

	MeshRenderer::~MeshRenderer() {
		SafeDealloc(mMaterial);
		SafeDealloc(mMesh);
	}

	void MeshRenderer::SetMaterial(Material* mat) {
		mMaterial = mat;
		mProgram = mMaterial->shader;
	}

	void MeshRenderer::SetMesh(Mesh* mesh) {
		mMesh = mesh;

		Buffer* buffer = new Buffer(BufferType::BufferVertex);
		VertexSemantic semantic = mMesh->GetVertexSemantic();
		size_t size = 0;

		buffer->Data(nullptr, mMesh->GetVertexCount() * sizeof(Vector3d) + mMesh->GetUvCount() * sizeof(Vector2d) + mMesh->GetNormalCount() * sizeof(Vector3d), mMesh->GetBufferUsage());
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

			index->Data(&mMesh->mFaces.front(), mMesh->GetFaceCount() * indexSize, mMesh->GetBufferUsage());
			mEbos.push_back(index);
			mVao->BindElements(*mEbos.front());
		}

		size_t offset = 0;
		if (semantic & VertexSemantic::SemanticPosition) {
			mVao->BindAttribute(mProgram->GetAttribute("position"), *mVbos.front(), 3, sizeof(Vector3d), offset);
			offset += mMesh->GetVertexCount() * sizeof(Vector3d);
		} 
		if (semantic & VertexSemantic::SemanticTexCoord) {
			mVao->BindAttribute(mProgram->GetAttribute("uvs"), *mVbos.front(), 2, sizeof(Vector2d), offset);
			offset += mMesh->GetUvCount() * sizeof(Vector2d);
		} 
		if (semantic & VertexSemantic::SemanticNormal) {
			mVao->BindAttribute(mProgram->GetAttribute("normal"), *mVbos.front(), 3, sizeof(Vector3d), offset);
			offset += mMesh->GetNormalCount() * sizeof(Vector3d);
		}
	}

	void MeshRenderer::Update(double deltaTime) {
	}

	void MeshRenderer::Render() {
		mProgram->Use();
		mProgram->SetUniform("model", mGameObject->GetTransform()->GetLocalToWorldMatrix());
		mProgram->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
		mProgram->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
		mProgram->SetUniform("viewPos", SceneManager::GetMainCamera()->GetTransform()->GetPosition());
			
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

		mProgram->SetUniform("dirLight.direction", Vector3d(-0.2f, -1.0f, -0.3f));
		mProgram->SetUniform("dirLight.ambient", SceneManager::GetGlobalLight()->ambient);
		mProgram->SetUniform("dirLight.diffuse", SceneManager::GetGlobalLight()->diffuse);
		mProgram->SetUniform("dirLight.specular", SceneManager::GetGlobalLight()->specular);
		
		mProgram->SetUniform("spotLight.position", SceneManager::GetMainCamera()->GetTransform()->GetPosition());
		mProgram->SetUniform("spotLight.direction", SceneManager::GetMainCamera()->GetTransform()->GetForward());
		mProgram->SetUniform("spotLight.ambient", Color(0.0f, 0.0f, 0.0f, 0.f));
		mProgram->SetUniform("spotLight.diffuse", Color(1.0f, 1.0f, 1.0f, 1.f));
		mProgram->SetUniform("spotLight.specular", Color(1.0f, 1.0f, 1.0f, 1.f));
		mProgram->SetUniform("spotLight.constant", 1.0f);
		mProgram->SetUniform("spotLight.linear", 0.09f);
		mProgram->SetUniform("spotLight.quadratic", 0.032f);
		mProgram->SetUniform("spotLight.cutOff", Math::Cos(Math::Radians(30.5f)));
		mProgram->SetUniform("spotLight.outerCutOff", Math::Cos(Math::Radians(45.0f)));

		mProgram->SetUniform("light.constant", SceneManager::GetGlobalLight()->constant);
		mProgram->SetUniform("light.linear", SceneManager::GetGlobalLight()->linear);
		mProgram->SetUniform("light.quadratic", SceneManager::GetGlobalLight()->quadratic);

		if (mMesh->mSemantic & VertexSemantic::SemanticFaces) {
			Graphics::DrawElements(*mVao, mPrimitive, 0, mMesh->GetFaceCount(), mMesh->GetIndexFormat());
		} else {
			Graphics::DrawArrays(*mVao, mPrimitive, 0, mMesh->GetVertexCount());
		}

		Graphics::BindTexture(0, NULL);
		Graphics::BindTexture(1, NULL);
		mProgram->UnUse();

#ifdef _DEBUG
		if (mIsDebugRendering) {
			DEBUG_PROGRAM->Use();
			DEBUG_PROGRAM->SetUniform("model", mGameObject->GetTransform()->GetLocalToWorldMatrix());
			DEBUG_PROGRAM->SetUniform("view", SceneManager::GetMainCamera()->GetWorldToCameraMatrix());
			DEBUG_PROGRAM->SetUniform("projection", SceneManager::GetMainCamera()->GetProjectionMatrix());
			if (mMesh->GetFaceCount() > 0) {
				Graphics::DrawElements(*mVao, mPrimitive, 0, mMesh->GetFaceCount(), mMesh->GetIndexFormat());
			} else {
				Graphics::DrawArrays(*mVao, mPrimitive, 0, mMesh->GetVertexCount());
			}
			DEBUG_PROGRAM->UnUse();
		}
#endif
	}

	bool MeshRenderer::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool MeshRenderer::Destroy() {
		return false;
	}
}