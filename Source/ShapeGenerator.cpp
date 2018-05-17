#include "ShapeGenerator.h"
#include "Mesh.h"
#include <vector>

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

	Mesh* ShapeGenerator::GenerateSphere(unsigned int subdivisions = 50) {
		Mesh* mesh = new Mesh();

		// https://schneide.wordpress.com/2016/07/15/generating-an-icosphere-in-c/
		const float X = 0.525731112119133606f;
		const float Z = 0.850650808352039932f;
		const float N = 0.f;

		static const float vertices[] = {
			-X, N, Z, 
			X, N, Z, 
			-X, N,-Z, 
			X, N, -Z, 
			N, Z, X, 
			N, Z, -X, 
			N, -Z, X, 
			N, -Z, -X,
			Z, X, N, 
			-Z, X, N, 
			Z, -X, N, 
			-Z, -X, N
		};

		static const unsigned int triangles[] = {
			0,4,1,
			0,9,4,
			9,5,4,
			4,5,8,
			4,8,1,
			8,10,1,
			8,3,10,
			5,3,8,
			5,2,3,
			2,7,3,
			7,10,3,
			7,6,10,
			7,11,6,
			11,0,6,
			0,1,6,
			6,1,10,
			9,0,11,
			9,11,2,
			9,2,5,
			7,2,11
		};

		for (int i = 0; i < subdivisions; i++) {

		}

		mesh->SetVertices(vertices);
		mesh->SetTriangles(triangles);

		return mesh;
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