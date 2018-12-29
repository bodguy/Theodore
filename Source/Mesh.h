/**
  @brief mesh for meshRenderer
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Mesh_h
#define Mesh_h

#include "Asset.h"
#include "BoneWeight.h"
#include "Bounds.h"
#include "Enumeration.h"
#include "Matrix4x4.h"
#include "Vector2d.h"
#include "Vector3d.h"
#include "Vector4d.h"
#include <vector>

namespace Theodore {
  class MeshRenderer;
  class SkinndedMeshRenderer;
  class Mesh : public Asset {
    friend class MeshRenderer;
    friend class SkinndedMeshRenderer;

  public:
    Mesh();
    virtual ~Mesh();

    template <typename T, size_t size>
    void SetVertices(const T (&verts)[size]);
    void SetVertices(const std::vector<Vector3d>& verts);

    template <typename T, size_t size>
    void SetUvs(const T (&uvs)[size]);
    void SetUvs(const std::vector<Vector2d>& uvs);

    template <typename T, size_t size>
    void SetNormals(const T (&normals)[size]);
    void SetNormals(const std::vector<Vector3d>& normals);

    template <typename T, size_t size>
    void SetTriangles(const T (&faces)[size]);
    void SetTriangles(const std::vector<unsigned int>& faces);

    int GetVertexCount() const { return mVertices.size(); }
    int GetUvCount() const { return mUvs.size(); }
    int GetNormalCount() const { return mNormals.size(); }
    int GetFaceCount() const { return mFaces.size(); }
    IndexFormat GetIndexFormat() const { return mFormat; }
    VertexSemantic GetVertexSemantic() const { return mSemantic; }
    BufferUsage GetBufferUsage() const { return mUsage; }
    std::vector<Vector3d>& GetVertexData() { return mVertices; }
    Bounds* GetBounds() { return &mBounds; }

    void SetBoneWeight(BoneWeight* bw);
    void RecalculateNormals();
    void RecalculateBounds();
    void MarkDynamic();

  protected:
    IndexFormat mFormat;
    VertexSemantic mSemantic;
    BufferUsage mUsage;
    std::vector<Vector3d> mVertices;
    std::vector<Vector2d> mUvs;
    std::vector<Vector2d> mUvs2;
    std::vector<Vector2d> mUvs3;
    std::vector<Vector2d> mUvs4;
    std::vector<Vector3d> mNormals;
    std::vector<Vector4d> mTangents;
    std::vector<unsigned int> mFaces;
    std::vector<BoneWeight> mBoneWeights;
    std::vector<Matrix4x4> mBindposes;
    // local space bounds
    Bounds mBounds;
  };

  template <typename T, size_t size>
  void Mesh::SetVertices(const T (&verts)[size]) {
    mSemantic = mSemantic | VertexSemantic::SemanticPosition;

    unsigned int stride = 3;
    for (unsigned int i = 0; i < size / stride; i++) {
      mVertices.push_back(
          Vector3d(verts[i * stride], verts[i * stride + 1], verts[i * stride + 2]));
    }
  }

  template <typename T, size_t size>
  void Mesh::SetUvs(const T (&uvs)[size]) {
    mSemantic = mSemantic | VertexSemantic::SemanticTexCoord;

    unsigned int stride = 2;
    for (unsigned int i = 0; i < size / stride; i++) {
      mUvs.push_back(Vector2d(uvs[i * stride], uvs[i * stride + 1]));
    }
  }

  template <typename T, size_t size>
  void Mesh::SetNormals(const T (&normals)[size]) {
    mSemantic = mSemantic | VertexSemantic::SemanticNormal;

    unsigned int stride = 3;
    for (unsigned int i = 0; i < size / stride; i++) {
      mNormals.push_back(
          Vector3d(normals[i * stride], normals[i * stride + 1], normals[i * stride + 2]));
    }
  }

  template <typename T, size_t size>
  void Mesh::SetTriangles(const T (&faces)[size]) {
    mSemantic = mSemantic | VertexSemantic::SemanticFaces;

    for (unsigned int i = 0; i < size; i++) {
      mFaces.push_back(faces[i]);
    }
  }
}

#endif /* Mesh_h */