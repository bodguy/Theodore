#include "ShapeGenerator.h"
#include "Mesh.h"

namespace Quark {
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

	ShapeGenerator::ShapeGenerator() {
	
	}

	ShapeGenerator::~ShapeGenerator() {
	
	}

	Mesh* ShapeGenerator::GeneratePlane(unsigned int dimensions) {
		Mesh* mesh = new Mesh();

		size_t vertexCount = dimensions * dimensions;
		std::vector<Vector3d> vertices;
		vertices.reserve(vertexCount);

		float half = dimensions * 0.5f;

		for (unsigned int i = 0; i < dimensions; i++) {
			for (unsigned int j = 0; j < dimensions; j++) {
				vertices.push_back(Vector3d(static_cast<float>(j - half), 0.f, static_cast<float>(i - half)));
			}
		}

		size_t faceCount = (dimensions - 1) * (dimensions - 1) * 2 * 3;
		std::vector<unsigned int> indices;
		indices.reserve(faceCount);

		for (unsigned int row = 0; row < dimensions - 1; row++) {
			for (unsigned int col = 0; col < dimensions - 1; col++) {
				indices.push_back(dimensions * row + col);
				indices.push_back(dimensions * row + col + dimensions);
				indices.push_back(dimensions * row + col + dimensions + 1);

				indices.push_back(dimensions * row + col);
				indices.push_back(dimensions * row + col + dimensions + 1);
				indices.push_back(dimensions * row + col + 1);
			}
		}

		mesh->SetVertices(vertices);
		mesh->SetTriangles(indices);
		mesh->RecalculateNormals();

		return mesh;
	}

	Mesh* ShapeGenerator::GenerateCube() {
		Mesh* mesh = new Mesh();

		static const float vertices[] = {
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

		static const float normals[] = {
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

	Mesh* ShapeGenerator::GenerateSphere(unsigned int subdivisions) {
		Mesh* mesh = new Mesh();

		float t = (1.f + std::sqrtf(5.f) / 2.f);
		std::vector<Vector3d> vertices = {
			Vector3d(-1.f, t, 0.f).Normalize(), Vector3d(1.f, t, 0.f).Normalize(), Vector3d(-1.f, -t, 0.f).Normalize(), Vector3d(1.f, -t, 0.f).Normalize(),
			Vector3d(0.f, -1.f, t).Normalize(), Vector3d(0.f, 1.f, t).Normalize(), Vector3d(0.f, -1.f, -t).Normalize(), Vector3d(0.f, 1.f, -t).Normalize(),
			Vector3d(t, 0.f, -1.f).Normalize(), Vector3d(t, 0.f, 1.f).Normalize(), Vector3d(-t, 0.f, -1.f).Normalize(), Vector3d(-t, 0.f, 1.f).Normalize()
		};

		std::vector<unsigned int> indices = {
			0, 11, 5,
			0, 5, 1,
			0, 1, 7,
			0, 7, 10,
			0, 10, 11,

			1, 5, 9,
			5, 11, 4,
			11, 10, 2,
			10, 7, 6,
			7, 1, 8,

			3, 9, 4,
			3, 4, 2,
			3, 2, 6,
			3, 6, 8,
			3, 8, 9,

			4, 9, 5,
			2, 4, 11,
			6, 2, 10,
			8, 6, 7,
			9, 8, 1
		};

		std::vector<Vector3d> normals;

		for (unsigned int i = 0; i < subdivisions; i++) {
			std::vector<unsigned int> indices2;
			for (unsigned int j = 0; j < indices.size() / 3; j++) {
				unsigned int a = Subdivide(indices[j * 3 + 0], indices[j * 3 + 1], vertices);
				unsigned int b = Subdivide(indices[j * 3 + 1], indices[j * 3 + 2], vertices);
				unsigned int c = Subdivide(indices[j * 3 + 2], indices[j * 3 + 0], vertices);

				indices2.push_back(indices[j * 3 + 0]);
				indices2.push_back(a);
				indices2.push_back(c);

				indices2.push_back(indices[j * 3 + 1]);
				indices2.push_back(b);
				indices2.push_back(a);

				indices2.push_back(indices[j * 3 + 2]);
				indices2.push_back(c);
				indices2.push_back(b);

				indices2.push_back(a);
				indices2.push_back(b);
				indices2.push_back(c);
			}
			indices = indices2;
		}

		Vector3d center = Vector3d(0.f, 0.f, 0.f);
		for (unsigned int i = 0; i < indices.size(); i++) {
			Vector3d norm = (vertices.at(indices[i])).Normalize();
			normals.push_back(norm);
		}

		mesh->SetVertices(vertices);
		mesh->SetTriangles(indices);
		mesh->SetNormals(normals);

		return mesh;
	}

	unsigned int ShapeGenerator::Subdivide(unsigned int p1, unsigned int p2, std::vector<Vector3d>& positions) {
		Vector3d middle = (positions[p1] + positions[p2]) / 2.f;
		positions.push_back(middle.Normalize());
		return positions.size() - 1;
	}

	Mesh* ShapeGenerator::GenerateCapsule() {
		Mesh* mesh = new Mesh();
		return mesh;
	}

	Mesh* ShapeGenerator::GenerateCylinder() {
		Mesh* mesh = new Mesh();
		return mesh;
	}
}