// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Mesh_h
#define Mesh_h

#include <vector>

#include "Asset/Asset.h"
#include "Component/BoneWeight.h"
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

		size_t GetVertexCount() const { return vertices.size(); }
    size_t GetTriangleCount() const { return triangles.size(); }
    bool IsVertexEmpty() const { return GetVertexCount() == 0; }
    IndexFormat GetIndexFormat() const { return indexFormat; }
    VertexSemantic GetVertexSemantic() const { return vertexSemantic; }
    BufferUsage GetBufferUsage() const { return bufferUsage; }
    std::vector<Vertex>& GetVertexData() { return vertices; }
    Bounds* GetBounds() { return &bounds; }

    void RecalculateNormals();
		void RecalculateTangents(unsigned int offset);
    void RecalculateBounds();
    void Triangulate(size_t npolys);
    void MarkDynamic();

    virtual bool LoadMesh(const std::string& fileName, MeshParseOption parseOption) { return false; }

  protected:
    IndexFormat indexFormat;
    VertexSemantic vertexSemantic;
    BufferUsage bufferUsage;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> triangles;
    std::vector<BoneWeight> boneWeights;
    std::vector<Matrix4x4> bindPoses;
    size_t materialId;
    Bounds bounds; // local space bounds
  };
}  // namespace Theodore

#endif /* Mesh_h */