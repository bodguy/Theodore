#ifndef PrimitiveData_h
#define PrimitiveData_h

namespace Quark {
	class Mesh;
	class PrimitiveData {
	public:
		static Mesh* GeneratePlane() {
			Mesh* mesh = new Mesh();
			return mesh;
		}

		static Mesh* GenerateCube() {
			Mesh* mesh = new Mesh();
			float one = 1.f;

			const float cubeVertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,

				// front face
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				// left face
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,

				// right face
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,

				// down face
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,

				// up face
				-1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,
			};

			const float cubeNormals[] = {
				0.0f,  0.0f, -1.0f,
				0.0f,  0.0f, -1.0f,
				0.0f,  0.0f, -1.0f,
				0.0f,  0.0f, -1.0f,
				0.0f,  0.0f, -1.0f,
				0.0f,  0.0f, -1.0f,

				0.0f,  0.0f,  1.0f,
				0.0f,  0.0f,  1.0f,
				0.0f,  0.0f,  1.0f,
				0.0f,  0.0f,  1.0f,
				0.0f,  0.0f,  1.0f,
				0.0f,  0.0f,  1.0f,

				-1.0f,  0.0f,  0.0f,
				-1.0f,  0.0f,  0.0f,
				-1.0f,  0.0f,  0.0f,
				-1.0f,  0.0f,  0.0f,
				-1.0f,  0.0f,  0.0f,
				-1.0f,  0.0f,  0.0f,

				1.0f,  0.0f,  0.0f,
				1.0f,  0.0f,  0.0f,
				1.0f,  0.0f,  0.0f,
				1.0f,  0.0f,  0.0f,
				1.0f,  0.0f,  0.0f,
				1.0f,  0.0f,  0.0f,

				0.0f, -1.0f,  0.0f,
				0.0f, -1.0f,  0.0f,
				0.0f, -1.0f,  0.0f,
				0.0f, -1.0f,  0.0f,
				0.0f, -1.0f,  0.0f,
				0.0f, -1.0f,  0.0f,

				0.0f,  1.0f,  0.0f,
				0.0f,  1.0f,  0.0f,
				0.0f,  1.0f,  0.0f,
				0.0f,  1.0f,  0.0f,
				0.0f,  1.0f,  0.0f,
				0.0f,  1.0f,  0.0f
			};
			
			return mesh;
		}

		static Mesh* GenerateSphere() {
			Mesh* mesh = new Mesh();
			return mesh;
		}

		static Mesh* GenerateCapsule() {
			Mesh* mesh = new Mesh();
			return mesh;
		}

		static Mesh* GenerateCylinder() {
			Mesh* mesh = new Mesh();
			return mesh;
		}

	private:
		PrimitiveData() {}
		~PrimitiveData() {}
	};
	

	const float boneVertices[] = {
		0.0f, 0.4f, 0.0f, // 0
		-0.4f, 0.0f, -0.4f, // 1
		0.4f, 0.0f, -0.4f, // 2
		0.0f, -3.f, 0.0f, // Base
		-0.4f, 0.0f, -0.4f, // 1
		-0.4f, 0.0f, 0.4f, // 4
		0.0f, 0.4f, 0.0f, // 0
		0.4f, 0.0f, -0.4f, // 2
		0.4f, 0.0f, 0.4f, // 3
		0.0f, 0.4f, 0.0f, // 0
		-0.4f, 0.0f, 0.4f, // 4
		0.0f, -3.f, 0.0f, // Base
		0.4f, 0.0f, 0.4f, // 3
		-0.4f, 0.0f, 0.4f // 4
	};

}

#endif /* PrimitiveData_h */