#ifndef ShapeGenerator_h
#define ShapeGenerator_h

namespace Quark {
	class Mesh;
	class ShapeGenerator {
	public:
		static Mesh* GeneratePlane(unsigned int dimensions = 50);
		static Mesh* GenerateCube();
		static Mesh* GenerateSphere(unsigned int subdivisions = 50);
		static Mesh* GenerateCapsule();
		static Mesh* GenerateCylinder();

	private:
		ShapeGenerator();
		~ShapeGenerator();
	};
}

#endif /* ShapeGenerator_h */