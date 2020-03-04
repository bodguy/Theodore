// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Mesh.h"

namespace Theodore {
  Mesh::Mesh()
  	: mFormat(IndexFormat::UInt32), mSemantic(VertexSemantic::SemanticNone), mUsage(BufferUsage::StaticDraw), mBounds(Vector3d::zero, Vector3d::one), mMaterialID(-1) {
    mType = AssetType::MeshType;
    mVertices.clear();
    mFaces.clear();
    mBoneWeights.clear();
    mBindposes.clear();
  }

  Mesh::~Mesh() {}

	void Mesh::SetVertexSemantic(const VertexSemantic vertexSemantic) {
  	mSemantic = vertexSemantic;
  }

  void Mesh::SetVertices(const std::vector<Vertex>& verts) {
    for (auto& v : verts) {
      mVertices.emplace_back(v);
    }
  }

  void Mesh::SetFaces(const std::vector<unsigned int>& faces) {
    for (auto& f : faces) {
      mFaces.emplace_back(f);
    }
  }

	void Mesh::PushVertex(const Vertex& vertex) {
  	mVertices.emplace_back(vertex);
  }

	void Mesh::PushFace(const unsigned int face) {
		mFaces.emplace_back(face);
	}

  void Mesh::RecalculateNormals() {
    static const unsigned int stride = 3;
    for (unsigned int i = 0; i < mFaces.size() / stride; i++) {
			Vector3d normal = Vector3d::CrossProduct(
				Vector3d(mVertices.at(mFaces[i * stride + 1]).position - mVertices.at(mFaces[i * stride]).position),
				Vector3d(mVertices.at(mFaces[i * stride + 2]).position - mVertices.at(mFaces[i * stride]).position)
			).Normalize();

			mVertices[i * stride].normal = normal;
			mVertices[i * stride + 1].normal = normal;
			mVertices[i * stride + 2].normal = normal;
    }
  }

	void Mesh::RecalculateTangents(unsigned int offset) {
		unsigned int offset_start = offset * 3;
		Vertex v1 = mVertices.at(offset_start);
		Vertex v2 = mVertices.at(offset_start + 1);
		Vertex v3 = mVertices.at(offset_start + 2);

		Vector3d e1 = v2.position - v1.position;
		Vector3d e2 = v3.position - v1.position;
		Vector2d delta1 = v2.texcoord - v1.texcoord;
		Vector2d delta2 = v3.texcoord - v1.texcoord;

		float f = 1.f / (delta1.x * delta2.y - delta2.x * delta1.y);

		Vector3d tangent;
		tangent.x = f * (delta2.y * e1.x - delta1.y * e2.x);
		tangent.y = f * (delta2.y * e1.y - delta1.y * e2.y);
		tangent.z = f * (delta2.y * e1.z - delta1.y * e2.z);
		tangent.Normalize();

		v1.tangent = tangent;
		v2.tangent = tangent;
		v3.tangent = tangent;

		mVertices[offset_start] = v1;
		mVertices[offset_start + 1] = v2;
		mVertices[offset_start + 2] = v3;
	}

  void Mesh::RecalculateBounds() {
    if (!mVertices.empty()) {
      Vector3d min, max;

      for (auto iter = mVertices.cbegin(); iter < mVertices.cend(); iter++) {
        if ((*iter).position.x < min.x) {
          min.x = (*iter).position.x;
        } else if ((*iter).position.x > max.x) {
          max.x = (*iter).position.x;
        }

        if ((*iter).position.y < min.y) {
          min.y = (*iter).position.y;
        } else if ((*iter).position.y > max.y) {
          max.y = (*iter).position.y;
        }

        if ((*iter).position.z < min.z) {
          min.z = (*iter).position.z;
        } else if ((*iter).position.z > max.z) {
          max.z = (*iter).position.z;
        }
      }

      mBounds.SetMinMax(min, max);
    }
  }

	void Mesh::Triangulate(size_t npolys) {
  	// TODO
  }

  void Mesh::MarkDynamic() { mUsage = BufferUsage::DynamicDraw; }
}  // namespace Theodore