#include "Mesh.h"
#include "../../../Helper/Utility.h"

namespace Theodore {
  Mesh::Mesh()
      : mFormat(IndexFormat::UInt32), mSemantic(VertexSemantic::SemanticNone),
        mUsage(BufferUsage::StaticDraw), mBounds(Vector3d::zero, Vector3d::one) {
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

  Mesh::~Mesh() {}

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
    mSemantic = mSemantic | VertexSemantic::SemanticFaces;

    for (auto& i : faces) {
      mFaces.push_back(i);
    }
  }

  void Mesh::SetBoneWeight(BoneWeight* bw) {}

  void Mesh::RecalculateNormals() {
    mSemantic = mSemantic | VertexSemantic::SemanticNormal;

    unsigned int stride = 3;
    for (unsigned int i = 0; i < mFaces.size() / stride; i++) {
      mNormals.push_back(
          Vector3d::CrossProduct(
              Vector3d(mVertices.at(mFaces[i * stride + 1]) - mVertices.at(mFaces[i * stride])),
              Vector3d(mVertices.at(mFaces[i * stride + 2]) - mVertices.at(mFaces[i * stride])))
              .Normalize());
    }
  }

  void Mesh::RecalculateBounds() {
    if (!mVertices.empty()) {
      std::vector<Vector3d>::const_iterator iter;
      Vector3d min, max;

      for (iter = mVertices.cbegin(); iter < mVertices.cend(); iter++) {
        if ((*iter).x < min.x) {
          min.x = (*iter).x;
        } else if ((*iter).x > max.x) {
          max.x = (*iter).x;
        }

        if ((*iter).y < min.y) {
          min.y = (*iter).y;
        } else if ((*iter).y > max.y) {
          max.y = (*iter).y;
        }

        if ((*iter).z < min.z) {
          min.z = (*iter).z;
        } else if ((*iter).z > max.z) {
          max.z = (*iter).z;
        }
      }

      mBounds.SetMinMax(min, max);
    }
  }

  void Mesh::MarkDynamic() { mUsage = BufferUsage::DynamicDraw; }
}