#include "Mesh.h"
#include "Utility.h"
#include "BoneWeight.h"

namespace Quark {
	Mesh::Mesh() : mFormat(IndexFormat::UInt32), mBoneWeights(nullptr) {
		mType = AssetType::MeshType;
	}

	Mesh::~Mesh(){
		SafeArrayDealloc(mBoneWeights);
	}

	void Mesh::SetVertices(const std::vector<Vector3d>& verts) {

	}

	void Mesh::SetNormals(const std::vector<Vector3d>& normals) {

	}

	void Mesh::SetTriangles(int* triangles) {

	}

	void Mesh::SetTriangles(const std::vector<int>& triangles) {

	}

	void Mesh::SetBoneWeight(BoneWeight* bw) {
		mBoneWeights = bw;
	}
}