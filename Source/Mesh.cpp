#include "Mesh.h"
#include "Utility.h"

namespace Quark {
	Mesh::Mesh() : mFormat(IndexFormat::UInt32), mSemantic(VertexSemantic::SemanticNone), mUsage(BufferUsage::StaticDraw) {
		mType = AssetType::MeshType;
		mVertices.clear();
		mUvs.clear();
		mUvs2.clear();
		mUvs3.clear();
		mUvs4.clear();
		mNormals.clear();
		mTangents.clear();
		mFaces.clear();
		mBoneWeights.clear();
		mBindposes.clear();
	}

	Mesh::~Mesh(){
		
	}

	void Mesh::SetVertices(const std::vector<Vector3d>& verts) {
		mSemantic = mSemantic | VertexSemantic::SemanticPosition;

		for (auto& i : verts) {
			mVertices.push_back(i);
		}
	}

	void Mesh::SetUvs(const std::vector<Vector2d>& uvs) {
		mSemantic = mSemantic | VertexSemantic::SemanticTexCoord;

		for (auto& i : uvs) {
			mUvs.push_back(i);
		}
	}

	void Mesh::SetNormals(const std::vector<Vector3d>& normals) {
		mSemantic = mSemantic | VertexSemantic::SemanticNormal;

		for (auto& i : normals) {
			mNormals.push_back(i);
		}
	}

	void Mesh::SetTriangles(const std::vector<unsigned int>& faces) {
		for (auto& i : faces) {
			mFaces.push_back(i);
		}
	}

	void Mesh::SetBoneWeight(BoneWeight* bw) {
	
	}

	void Mesh::RecalculateNormals() {
		mSemantic = mSemantic | VertexSemantic::SemanticNormal;

		unsigned int stride = 3;
		for (unsigned int i = 0; i < mFaces.size() / stride; i++) {
			mNormals.push_back( Vector3d::CrossProduct(Vector3d(mVertices.at(mFaces[i * stride + 1]) - mVertices.at(mFaces[i * stride])), 
														Vector3d(mVertices.at(mFaces[i * stride + 2]) - mVertices.at(mFaces[i * stride]))
													  ).Normalize()
			);
		}
	}

	void Mesh::RecalculateBounds() {

	}

	void Mesh::MarkDynamic() {
		mUsage = BufferUsage::DynamicDraw;
	}
}