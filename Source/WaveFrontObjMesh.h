#ifndef WaveFrontObjMesh_h
#define WaveFrontObjMesh_h

#include "Mesh.h"

namespace Quark {
	class WaveFrontObjMesh : public Mesh {
	public:
		WaveFrontObjMesh();
		virtual ~WaveFrontObjMesh();

		bool LoadObj(const std::string& filepath);

	private:
		void ProcessVertex(const std::vector<std::string>& splitStr, const std::vector<Vector2d>& uvsIn, const std::vector<Vector3d>& normalsIn);
	};
}

#endif /* ObjMesh_h */