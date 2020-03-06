// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef WaveFrontObjMesh_h
#define WaveFrontObjMesh_h

#include "Graphics/Enumeration.h"
#include "Asset/Mesh.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"
#include "MeshGroup.h"
#include <string>

namespace Theodore {
	struct VertexIndex {
		VertexIndex() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
		explicit VertexIndex(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
		VertexIndex(int vidx, int vtidx, int vnidx) : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
		int v_idx, vt_idx, vn_idx;
	};

	struct Triangle {
		Triangle() : vertexIndices() { vertexIndices.clear(); }
		std::vector<VertexIndex> vertexIndices;
	};

	struct PrimitiveGroup {
		PrimitiveGroup() : triangles() { triangles.clear(); }
		bool IsEmpty() const { return triangles.empty(); }
		std::vector<Triangle> triangles;
	};

	class MeshChunk {
	public:
		MeshChunk() : name(), vertices(), materialId(-1) { vertices.clear(); }
		size_t GetVertexCount() const { return vertices.size(); }
		size_t GetTriangleCount() const { return indices.size(); }
		bool IsVertexEmpty() const { return GetVertexCount() == 0; }
		void SetName(const std::string& name) { this->name = name; }
		void AppendVertex(Vertex& vertex) { vertices.emplace_back(vertex); }
		void AppendIndex(unsigned int index) { indices.emplace_back(index); }
		void SetMaterialId(int materialId) { this->materialId = materialId; }

	private:
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		int materialId;
	};

	class Texture;
  class WaveFrontObjMesh : public Mesh {
  public:
		WaveFrontObjMesh();
		virtual ~WaveFrontObjMesh() override;

		virtual bool LoadMesh(const std::string& fileName, MeshParseOption parseOption) override;

	private:
		bool ParsePrimitive(MeshChunk* mesh, const PrimitiveGroup& primitive, MeshParseOption option, const int materialId,
												const std::vector<Vector3d>& vertices, const std::vector<Vector2d>& texcoords, const std::vector<Vector3d>& normals,
												const std::string& name, const std::string& default_name);
		bool ParseIndices(const char** token, int vsize, int vnsize, int vtsize, VertexIndex* result);
		TextureFace ParseTextureFace(const char** token);
		bool ParseTexture(Texture& tex, const char* token);
		bool LoadMaterial(MeshGroup& meshGroup, std::unordered_map<std::string, int>& materialMap, std::istream& inputStream);
		bool ParseMaterial(MeshGroup& meshGroup, const std::string& materialName, std::unordered_map<std::string, int>& materialMap);

		static void SplitWithToken(std::vector<std::string>& elems, const char* delims, const char** token);
		std::string ParseString(const char** token) const;
		float ParseReal(const char** token, float defaultValue) const;
		void ParseReal2(Vector2d& vt, const char** token, float defaultX = 0.f, float defaultY = 0.f) const;
		void ParseReal3(Vector3d& vn, const char** token, float defaultX = 0.f, float defaultY = 0.f, float defaultZ = 0.f) const;
		int ParseInt(const char **token) const;
		bool ParseOnOff(const char** token, bool defaultValue) const;
		bool FixIndex(int idx, int n, int* ret) const;

  private:
  	MeshGroup meshGroup;
  };
}  // namespace Theodore

#endif /* ObjMesh_h */