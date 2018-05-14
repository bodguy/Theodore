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

	}

	void Mesh::SetUvs(const std::vector<Vector2d>& verts) {

	}

	void Mesh::SetNormals(const std::vector<Vector3d>& normals) {

	}

	void Mesh::SetTriangles(const std::vector<int>& triangles) {

	}

	void Mesh::SetBoneWeight(BoneWeight* bw) {
	
	}

	void Mesh::RecalculateNormals() {
		unsigned int stride = 6;
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