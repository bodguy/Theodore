#ifndef WaveFrontObjMesh_h
#define WaveFrontObjMesh_h

#include "Mesh.h"
#include "Enumeration.h"

namespace Quark {
	class WaveFrontObjMesh : public Mesh {
	public:
		WaveFrontObjMesh();
		virtual ~WaveFrontObjMesh();

		bool LoadObj(const std::string& filepath);

	private:
		void ProcessVertex(FILE* file, const std::vector<Vector2d>& uvsIn, const std::vector<Vector3d>& normalsIn, States states);
	};
}

#endif /* ObjMesh_h */