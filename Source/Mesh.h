#ifndef Mesh_h
#define Mesh_h

#include "Enumeration.h"
#include "Vector3d.h"
#include "Vector2d.h"
#include "Asset.h"
#include <vector>

namespace Quark {
	class MeshRenderer; class SkinndedMeshRenderer; class BoneWeight;
	class Mesh : public Asset {
		friend class MeshRenderer;
		friend class SkinndedMeshRenderer;
	public:
		Mesh();
		virtual ~Mesh();

		template<typename T, size_t size>
		void SetVertices(const T(&verts)[size]);
		void SetVertices(const std::vector<Vector3d>& verts);

		template<typename T, size_t size>
		void SetNormals(const T(&normals)[size]);
		void SetNormals(const std::vector<Vector3d>& normals);

		void SetTriangles(int* triangles);
		void SetTriangles(const std::vector<int>& triangles);

		int GetVertexCount() const { return mVertices.size(); }
		int GetUvCount() const { return mUvs.size(); }
		int GetNormalCount() const { return mNormals.size(); }
		int GetFaceCount() const { return mFaces.size(); }
		IndexFormat GetIndexFormat() const { return mFormat; }

		void SetBoneWeight(BoneWeight* bw);

	private:
		IndexFormat mFormat;
		std::vector<Vector3d> mVertices;
		std::vector<Vector2d> mUvs;
		std::vector<Vector3d> mNormals;
		std::vector<unsigned int> mFaces;
		BoneWeight* mBoneWeights;
	};

	template<typename T, size_t size>
	void Mesh::SetVertices(const T(&verts)[size]) {
		/*int stride = 3;
		mVertexCount = size / stride;
		for (int i = 0; i < mVertexCount; i++) {
			mStream.Vec3(Vector3d(verts[i * stride], verts[i * stride + 1], verts[i * stride + 2]));
		}
		mVbo->Data(mStream.Pointer(), mStream.Size(), Enumeration::StaticDraw);*/
	}

	template<typename T, size_t size>
	void Mesh::SetNormals(const T(&normals)[size]) {
		//int stride = 3;
		//mNormalCount = size / stride;
		//for (int i = 0; i < mNormalCount; i++) {
		//	mStream.Vec3(Vector3d(normals[i * stride], normals[i * stride + 1], normals[i * stride + 2]));
		//}
	}
}

#endif /* Mesh_h */