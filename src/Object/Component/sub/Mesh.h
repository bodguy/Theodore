// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Mesh_h
#define Mesh_h

#include <vector>

#include "Asset/Asset.h"
#include "BoneWeight.h"
#include "Geometry/Bounds.h"
#include "Graphics/Enumeration.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"
#include "Math/Vector4d.h"

namespace Theodore {
	struct Vertex {
		Vertex() : position(), texcoord(), normal(), tangent() {}
		Vector3d position;
		Vector2d texcoord;
		Vector3d normal;
		Vector3d tangent;
	};

  class MeshRenderer;
  class SkinndedMeshRenderer;
  class Mesh : public Asset {
    friend class MeshRenderer;
    friend class SkinndedMeshRenderer;

  public:
    Mesh();
    virtual ~Mesh() override;

    void SetVertexSemantic(const VertexSemantic vertexSemantic);
    void SetVertices(const std::vector<Vertex>& verts);
    void SetFaces(const std::vector<unsigned int>& faces);
    void PushVertex(const Vertex& vertex);
    void PushFace(const unsigned int face);

		size_t GetVertexCount() const { return mVertices.size(); }
    size_t GetFaceCount() const { return mFaces.size(); }
    IndexFormat GetIndexFormat() const { return mFormat; }
    VertexSemantic GetVertexSemantic() const { return mSemantic; }
    BufferUsage GetBufferUsage() const { return mUsage; }
    std::vector<Vertex>& GetVertexData() { return mVertices; }
    Bounds* GetBounds() { return &mBounds; }

    void RecalculateNormals();
		void RecalculateTangents(unsigned int offset);
    void RecalculateBounds();
    void Triangulate(size_t npolys);
    void MarkDynamic();

  protected:
    IndexFormat mFormat;
    VertexSemantic mSemantic;
    BufferUsage mUsage;
		std::vector<Vertex> mVertices;
		std::vector<unsigned int> mFaces;
    std::vector<BoneWeight> mBoneWeights;
    std::vector<Matrix4x4> mBindposes;
    size_t mMaterialID;
    Bounds mBounds; // local space bounds
  };
}  // namespace Theodore

#endif /* Mesh_h */