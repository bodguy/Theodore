#include "Mesh.h"
#include "Utility.h"
#include "BoneWeight.h"

namespace Quark {
	Mesh::Mesh() : mFormat(IndexFormat::UInt32), mSemantic(VertexSemantic::SemanticNone), mBoneWeights(nullptr) {
		mType = AssetType::MeshType;
	}

	Mesh::~Mesh(){
		SafeArrayDealloc(mBoneWeights);
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
		mBoneWeights = bw;
	}

	void Mesh::ReCalculateNormals() {
		unsigned int stride = 6;
		for (unsigned int i = 0; i < mFaces.size() / stride; i++) {
			mNormals.push_back( Vector3d::CrossProduct(Vector3d(mVertices.at(mFaces[i * stride + 1]) - mVertices.at(mFaces[i * stride])), 
														Vector3d(mVertices.at(mFaces[i * stride + 2]) - mVertices.at(mFaces[i * stride]))
													  ).Normalize()
			);
		}
	}
}