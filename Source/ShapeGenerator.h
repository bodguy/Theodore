#ifndef ShapeGenerator_h
#define ShapeGenerator_h

#include <vector>
#include <map>
#include "Vector3d.h"

namespace Quark {
	class Mesh;
	class ShapeGenerator {
	public:
		static Mesh* GeneratePlane(unsigned int dimensions = 50);
		static Mesh* GenerateCube();
		static Mesh* GenerateSphere(unsigned int subdivisions = 3);
		static Mesh* GenerateCapsule();
		static Mesh* GenerateCylinder();

	private:
		ShapeGenerator();
		~ShapeGenerator();

		static unsigned int Subdivide(unsigned int p1, unsigned int p2, std::vector<Vector3d>& positions);
	};
}

#endif /* ShapeGenerator_h */