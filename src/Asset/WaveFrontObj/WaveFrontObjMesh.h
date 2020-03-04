// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef WaveFrontObjMesh_h
#define WaveFrontObjMesh_h

#include <string>
#include "Graphics/Enumeration.h"
#include "Object/Component/sub/Mesh.h"
#include "Math/Vector2d.h"
#include "Math/Vector3d.h"

namespace Theodore {
	struct VertexIndex {
		VertexIndex() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}
		explicit VertexIndex(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}
		VertexIndex(int vidx, int vtidx, int vnidx) : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
		int v_idx, vt_idx, vn_idx;
	};

	struct Face {
		Face() : vertex_indices() { vertex_indices.clear(); }
		std::vector<VertexIndex> vertex_indices;
	};

	struct PrimitiveGroup {
		PrimitiveGroup() : faces() { faces.clear(); }
		bool is_empty() const { return faces.empty(); }
		std::vector<Face> faces;
	};

  class WaveFrontObjMesh : public Mesh {
  public:
		WaveFrontObjMesh() : Mesh() {}
		virtual ~WaveFrontObjMesh() override {}

	private:
		std::string ParseString(const char** token) const;
		float ParseReal(const char** token, float default_value) const;
		void ParseReal2(Vector2d& vt, const char** token, float default_x = 0.f, float default_y = 0.f) const;
		void ParseReal3(Vector3d& vn, const char** token, float default_x = 0.f, float default_y = 0.f, float default_z = 0.f) const;
		int ParseInt(const char **token) const;
		bool ParseOnOff(const char** token, bool default_value) const;
		bool FixIndex(int idx, int n, int* ret) const ;
		bool IsSpace(char x);
  };
}  // namespace Theodore

#endif /* ObjMesh_h */