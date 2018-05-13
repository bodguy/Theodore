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

			const float vertices[] = {
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,

				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,

				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,

				-1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,
			};

			const float normals[] = {
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

			mesh->SetVertices(vertices);
			mesh->SetNormals(normals);

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