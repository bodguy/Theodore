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
		void SetUvs(const T(&uvs)[size]);
		void SetUvs(const std::vector<Vector2d>& verts);

		template<typename T, size_t size>
		void SetNormals(const T(&normals)[size]);
		void SetNormals(const std::vector<Vector3d>& normals);

		template<typename T, size_t size>
		void SetTriangles(const T(&faces)[size]);
		void SetTriangles(const std::vector<int>& triangles);

		int GetVertexCount() const { return mVertices.size(); }
		int GetUvCount() const { return mUvs.size(); }
		int GetNormalCount() const { return mNormals.size(); }
		int GetFaceCount() const { return mFaces.size(); }
		IndexFormat GetIndexFormat() const { return mFormat; }
		VertexSemantic GetSemantic() const { return mSemantic; }

		void SetBoneWeight(BoneWeight* bw);
		void ReCalculateNormals();

	private:
		IndexFormat mFormat;
		VertexSemantic mSemantic;
		std::vector<Vector3d> mVertices;
		std::vector<Vector2d> mUvs;
		std::vector<Vector3d> mNormals;
		std::vector<unsigned int> mFaces;
		BoneWeight* mBoneWeights;
	};

	template<typename T, size_t size>
	void Mesh::SetVertices(const T(&verts)[size]) {
		mSemantic |= VertexSemantic::SemanticPosition;

		unsigned int stride = 3;
		for (unsigned int i = 0; i < size / stride; i++) {
			mVertices.push_back(Vector3d(verts[i * stride], verts[i * stride + 1], verts[i * stride + 2]));
		}
	}

	template<typename T, size_t size>
	void Mesh::SetUvs(const T(&uvs)[size]) {
		mSemantic |= VertexSemantic::SemanticTexCoord;

		unsigned int stride = 2;
		for (unsigned int i = 0; i < size / stride; i++) {
			mUvs.push_back(Vector2d(uvs[i * stride], uvs[i * stride + 1]));
		}
	}

	template<typename T, size_t size>
	void Mesh::SetNormals(const T(&normals)[size]) {
		mSemantic |= VertexSemantic::SemanticNormal;

		unsigned int stride = 3;
		for (unsigned int i = 0; i < size / 3; i++) {
			mNormals.push_back(Vector3d(normals[i * stride], normals[i * stride + 1], normals[i * stride + 2]));
		}
	}

	template<typename T, size_t size>
	void Mesh::SetTriangles(const T(&faces)[size]) {
		for (unsigned int i = 0; i < size; i++) {
			mFaces.push_back(faces[i]);
		}
	}
}

#endif /* Mesh_h */