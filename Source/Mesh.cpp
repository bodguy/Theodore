#include "Mesh.h"
#include "Utility.h"
#include "BoneWeight.h"

namespace Quark {
	Mesh::Mesh() : mFormat(Enumeration::UInt32), mVertexCount(0), mNormalCount(0), mVertexBufferCount(0), mBoneWeights(nullptr) {
		mType = Enumeration::MeshType;
	}

	Mesh::~Mesh(){
		SafeArrayDealloc(mBoneWeights);
	}

	void Mesh::SetVertices(const std::vector<Vector3d>& verts) {

	}

	void Mesh::SetNormals(const std::vector<Vector3d>& normals) {

	}

	void Mesh::SetTriangles(int* triangles, Enumeration::IndexFormat format) {

	}

	void Mesh::SetTriangles(const std::vector<int>& triangles, Enumeration::IndexFormat format) {

	}

	void Mesh::SetBoneWeight(BoneWeight* bw) {
		mBoneWeights = bw;
	}
}