#include "ShapeGenerator.h"
#include "../../../Math/Math.h"
#include "Mesh.h"

namespace Theodore {
  const float bones[] = {
      0.0f,  0.4f, 0.0f,  // 0
      -0.4f, 0.0f, -0.4f, // 1
      0.4f,  0.0f, -0.4f, // 2
      0.0f,  -3.f, 0.0f,  // Base
      -0.4f, 0.0f, -0.4f, // 1
      -0.4f, 0.0f, 0.4f,  // 4
      0.0f,  0.4f, 0.0f,  // 0
      0.4f,  0.0f, -0.4f, // 2
      0.4f,  0.0f, 0.4f,  // 3
      0.0f,  0.4f, 0.0f,  // 0
      -0.4f, 0.0f, 0.4f,  // 4
      0.0f,  -3.f, 0.0f,  // Base
      0.4f,  0.0f, 0.4f,  // 3
      -0.4f, 0.0f, 0.4f   // 4
  };

  static int patchdata[][16] = {
      /* rim */
      {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
      /* body */
      {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27},
      {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
      /* lid */
      {
          96,
          96,
          96,
          96,
          97,
          98,
          99,
          100,
          101,
          101,
          101,
          101,
          0,
          1,
          2,
          3,
      },
      {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117},
      /* bottom */
      {118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120, 40, 39, 38, 37},
      /* handle */
      {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56},
      {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 28, 65, 66, 67},
      /* spout */
      {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83},
      {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95}};

  static float cpdata[][3] = {{0.2, 0, 2.7},
                              {0.2, -0.112, 2.7},
                              {0.112, -0.2, 2.7},
                              {0, -0.2, 2.7},
                              {1.3375, 0, 2.53125},
                              {1.3375, -0.749, 2.53125},
                              {0.749, -1.3375, 2.53125},
                              {0, -1.3375, 2.53125},
                              {1.4375, 0, 2.53125},
                              {1.4375, -0.805, 2.53125},
                              {0.805, -1.4375, 2.53125},
                              {0, -1.4375, 2.53125},
                              {1.5, 0, 2.4},
                              {1.5, -0.84, 2.4},
                              {0.84, -1.5, 2.4},
                              {0, -1.5, 2.4},
                              {1.75, 0, 1.875},
                              {1.75, -0.98, 1.875},
                              {0.98, -1.75, 1.875},
                              {0, -1.75, 1.875},
                              {2, 0, 1.35},
                              {2, -1.12, 1.35},
                              {1.12, -2, 1.35},
                              {0, -2, 1.35},
                              {2, 0, 0.9},
                              {2, -1.12, 0.9},
                              {1.12, -2, 0.9},
                              {0, -2, 0.9},
                              {-2, 0, 0.9},
                              {2, 0, 0.45},
                              {2, -1.12, 0.45},
                              {1.12, -2, 0.45},
                              {0, -2, 0.45},
                              {1.5, 0, 0.225},
                              {1.5, -0.84, 0.225},
                              {0.84, -1.5, 0.225},
                              {0, -1.5, 0.225},
                              {1.5, 0, 0.15},
                              {1.5, -0.84, 0.15},
                              {0.84, -1.5, 0.15},
                              {0, -1.5, 0.15},
                              {-1.6, 0, 2.025},
                              {-1.6, -0.3, 2.025},
                              {-1.5, -0.3, 2.25},
                              {-1.5, 0, 2.25},
                              {-2.3, 0, 2.025},
                              {-2.3, -0.3, 2.025},
                              {-2.5, -0.3, 2.25},
                              {-2.5, 0, 2.25},
                              {-2.7, 0, 2.025},
                              {-2.7, -0.3, 2.025},
                              {-3, -0.3, 2.25},
                              {-3, 0, 2.25},
                              {-2.7, 0, 1.8},
                              {-2.7, -0.3, 1.8},
                              {-3, -0.3, 1.8},
                              {-3, 0, 1.8},
                              {-2.7, 0, 1.575},
                              {-2.7, -0.3, 1.575},
                              {-3, -0.3, 1.35},
                              {-3, 0, 1.35},
                              {-2.5, 0, 1.125},
                              {-2.5, -0.3, 1.125},
                              {-2.65, -0.3, 0.9375},
                              {-2.65, 0, 0.9375},
                              {-2, -0.3, 0.9},
                              {-1.9, -0.3, 0.6},
                              {-1.9, 0, 0.6},
                              {1.7, 0, 1.425},
                              {1.7, -0.66, 1.425},
                              {1.7, -0.66, 0.6},
                              {1.7, 0, 0.6},
                              {2.6, 0, 1.425},
                              {2.6, -0.66, 1.425},
                              {3.1, -0.66, 0.825},
                              {3.1, 0, 0.825},
                              {2.3, 0, 2.1},
                              {2.3, -0.25, 2.1},
                              {2.4, -0.25, 2.025},
                              {2.4, 0, 2.025},
                              {2.7, 0, 2.4},
                              {2.7, -0.25, 2.4},
                              {3.3, -0.25, 2.4},
                              {3.3, 0, 2.4},
                              {2.8, 0, 2.475},
                              {2.8, -0.25, 2.475},
                              {3.525, -0.25, 2.49375},
                              {3.525, 0, 2.49375},
                              {2.9, 0, 2.475},
                              {2.9, -0.15, 2.475},
                              {3.45, -0.15, 2.5125},
                              {3.45, 0, 2.5125},
                              {2.8, 0, 2.4},
                              {2.8, -0.15, 2.4},
                              {3.2, -0.15, 2.4},
                              {3.2, 0, 2.4},
                              {0, 0, 3.15},
                              {0.8, 0, 3.15},
                              {0.8, -0.45, 3.15},
                              {0.45, -0.8, 3.15},
                              {0, -0.8, 3.15},
                              {0, 0, 2.85},
                              {1.4, 0, 2.4},
                              {1.4, -0.784, 2.4},
                              {0.784, -1.4, 2.4},
                              {0, -1.4, 2.4},
                              {0.4, 0, 2.55},
                              {0.4, -0.224, 2.55},
                              {0.224, -0.4, 2.55},
                              {0, -0.4, 2.55},
                              {1.3, 0, 2.55},
                              {1.3, -0.728, 2.55},
                              {0.728, -1.3, 2.55},
                              {0, -1.3, 2.55},
                              {1.3, 0, 2.4},
                              {1.3, -0.728, 2.4},
                              {0.728, -1.3, 2.4},
                              {0, -1.3, 2.4},
                              {0, 0, 0},
                              {1.425, -0.798, 0},
                              {1.5, 0, 0.075},
                              {1.425, 0, 0},
                              {0.798, -1.425, 0},
                              {0, -1.5, 0.075},
                              {0, -1.425, 0},
                              {1.5, -0.84, 0.075},
                              {0.84, -1.5, 0.075}};

  static float tex[2][2][2] = {{{0, 0}, {1, 0}}, {{0, 1}, {1, 1}}};

  Mesh* ShapeGenerator::GeneratePlane(unsigned int dimensions) {
    Mesh* mesh = new Mesh();

    size_t vertexCount = dimensions * dimensions;
    std::vector<Vector3d> vertices;
    std::vector<Vector3d> normals;
    vertices.reserve(vertexCount);
    normals.reserve(vertexCount);

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

    static const float vertices[] = {-1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f,

                                     -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                                     -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,

                                     1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,

                                     -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,

                                     -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f};

    static const float normals[] = {0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,

                                    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

                                    -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

                                    1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

                                    0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,

                                    0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f};

    static const float uvs[] = {0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
                                0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f,
                                0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

    mesh->SetVertices(vertices);
    mesh->SetNormals(normals);
    mesh->SetUvs(uvs);

    return mesh;
  }

  Mesh* ShapeGenerator::GenerateIcoSphere(unsigned int subdivisions) {
    Mesh* mesh = new Mesh();

    float t = (1.f + Math::Sqrt(5.f) / 2.f);
    std::vector<Vector3d> vertices = {Vector3d(-1.f, t, 0.f).Normalize(), Vector3d(1.f, t, 0.f).Normalize(), Vector3d(-1.f, -t, 0.f).Normalize(), Vector3d(1.f, -t, 0.f).Normalize(),
                                      Vector3d(0.f, -1.f, t).Normalize(), Vector3d(0.f, 1.f, t).Normalize(), Vector3d(0.f, -1.f, -t).Normalize(), Vector3d(0.f, 1.f, -t).Normalize(),
                                      Vector3d(t, 0.f, -1.f).Normalize(), Vector3d(t, 0.f, 1.f).Normalize(), Vector3d(-t, 0.f, -1.f).Normalize(), Vector3d(-t, 0.f, 1.f).Normalize()};

    std::vector<unsigned int> indices = {0, 11, 5, 0, 5,  1,  0,  1,  7,  0,  7, 10, 0, 10, 11,

                                         1, 5,  9, 5, 11, 4,  11, 10, 2,  10, 7, 6,  7, 1,  8,

                                         3, 9,  4, 3, 4,  2,  3,  2,  6,  3,  6, 8,  3, 8,  9,

                                         4, 9,  5, 2, 4,  11, 6,  2,  10, 8,  6, 7,  9, 8,  1};

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

    for (Vector3d& v : vertices) {
      normals.push_back(v);
    }

    mesh->SetVertices(vertices);
    mesh->SetTriangles(indices);
    mesh->SetNormals(normals);

    return mesh;
  }

  Mesh* ShapeGenerator::GenerateUVSphere() {
    Mesh* mesh = new Mesh();
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

    std::vector<Vector3d> vertices;
    std::vector<Vector3d> normals;

    int length = 5;
    int numSteps = 40;
    float radius = 1.f;

    float hl = length * 0.5f;
    float a = 0.0f;
    float step = Math::pi * 2.f / (float)numSteps;

    for (int i = 0; i <= numSteps; ++i) {
      float x = Math::Cos(a) * radius;
      float y = Math::Sin(a) * radius;

      normals.push_back(Vector3d(x / radius, y / radius, 0.f).Normalize());
      vertices.push_back(Vector3d(x, y, -hl));
      normals.push_back(Vector3d(x / radius, y / radius, 0.f).Normalize());
      vertices.push_back(Vector3d(x, y, hl));

      a += step;
    }
    // vertices.push_back(Vector3d(0.f, 0.f, -hl));
    // vertices.push_back(Vector3d(0.f, 0.f, hl));

    mesh->SetVertices(vertices);
    mesh->SetNormals(normals);

    return mesh;
  }

  Mesh* ShapeGenerator::GenerateTorus(int numc, int numt) {
    Mesh* mesh = new Mesh();

    std::vector<Vector3d> vertices;
    std::vector<Vector3d> normals;

    float twopi = 2.f * Math::pi;
    float radius = 0.3f;

    for (int i = 0; i < numc; i++) {
      for (int j = 0; j <= numt; j++) {
        for (int k = 1; k >= 0; k--) {
          float s = (i + k) % numc + 0.5f;
          float t = (float)(j % numt);

          float x = (1 + radius * Math::Cos(s * twopi / numc)) * Math::Cos(t * twopi / numt);
          float y = (1 + radius * Math::Cos(s * twopi / numc)) * Math::Sin(t * twopi / numt);
          float z = radius * Math::Sin(s * twopi / numc);
          vertices.push_back(Vector3d(x, y, z));
          normals.push_back(Vector3d(x, y, z).Normalize());
        }
      }
    }

    mesh->SetVertices(vertices);
    mesh->SetNormals(normals);

    return mesh;
  }

  Mesh* ShapeGenerator::GenerateCone(float radius, float height) {
    Mesh* mesh = new Mesh();

    std::vector<Vector3d> vertices;
    std::vector<Vector3d> normals;

    float phi = 0.f;
    int nPhi = 100;
    float dPhi = 2.f * Math::pi / (nPhi - 1);

    vertices.push_back(Vector3d(0.f, height, 0.f));
    normals.push_back(Vector3d(0.f, height, 0.f));
    for (int i = 0; i < nPhi; i++) {
      vertices.push_back(Vector3d(Math::Cos(phi) * radius, -height, Math::Sin(phi) * radius));
      normals.push_back(Vector3d(height * Math::Cos(phi), radius, height * Math::Sin(phi)));
      phi += dPhi;
    }

    mesh->SetVertices(vertices);
    mesh->SetNormals(normals);

    return mesh;
  }

  Mesh* ShapeGenerator::GenerateUtahTeapot(unsigned int tesselation) {
    Mesh* mesh = new Mesh();

    /*unsigned int numVertices = 32 * (tesselation + 1) * (tesselation + 1);
    unsigned int faces = tesselation * tesselation * 32;
    std::vector<Vector3d> vertices; vertices.reserve(numVertices);
    std::vector<Vector2d> uvs; uvs.reserve(numVertices);
    std::vector<Vector3d> normals; normals.reserve(numVertices);
    std::vector<unsigned int> indices; indices.reserve(faces * 6);

    GeneratePatches(vertices, normals, uvs, indices, tesselation);
    MoveLid(tesselation, vertices, lidTransform);

    for (unsigned int i = 0; i < numVertices; i++) {
      v.UV = Vector2d((vertices[i * 3 + 0] + 1.0f) / 2.0f, (vertices[i * 3 + 1] + 1.0f) / 2.0f);
    }*/

    return mesh;
  }

  // void ShapeGenerator::GeneratePatches(float * v, float * n, float * tc, unsigned short* el, int
  // grid) {
  //	float * B = new float[4 * (grid + 1)];  // Pre-computed Bernstein basis functions
  //	float * dB = new float[4 * (grid + 1)]; // Pre-computed derivitives of basis functions

  //	int idx = 0, elIndex = 0, tcIndex = 0;

  //	// Pre-compute the basis functions  (Bernstein polynomials)
  //	// and their derivatives
  //	ComputeBasisFunctions(B, dB, grid);

  //	// Build each patch
  //	// The rim
  //	BuildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  //	// The body
  //	BuildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  //	BuildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  //	// The lid
  //	BuildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  //	BuildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  //	// The bottom
  //	BuildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
  //	// The handle
  //	BuildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  //	BuildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  //	// The spout
  //	BuildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
  //	BuildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

  //	delete[] B;
  //	delete[] dB;
  //}

  // void ShapeGenerator::MoveLid(int grid, float *v, Matrix4x4 lidTransform) {
  //	int start = 3 * 12 * (grid + 1) * (grid + 1);
  //	int end = 3 * 20 * (grid + 1) * (grid + 1);

  //	for (int i = start; i < end; i += 3) {
  //		Vector4d vert = Vector4d(v[i], v[i + 1], v[i + 2], 1.0f);
  //		vert = lidTransform * vert;
  //		v[i] = vert.x;
  //		v[i + 1] = vert.y;
  //		v[i + 2] = vert.z;
  //	}
  //}

  // void ShapeGenerator::BuildPatchReflect(int patchNum, float *B, float *dB, float *v, float *n,
  // float *tc, unsigned short *el, int &index, int &elIndex, int &tcIndex, int grid, bool reflectX,
  // bool reflectY) {
  //	Vector3d patch[4][4];
  //	Vector3d patchRevV[4][4];
  //	GetPatch(patchNum, patch, false);
  //	GetPatch(patchNum, patchRevV, true);

  //	// Patch without modification
  //	BuildPatch(patch, B, dB, v, n, tc, el, index, elIndex, tcIndex, grid, glm::mat3(1.0f), true);

  //	// Patch reflected in x
  //	if (reflectX) {
  //		BuildPatch(patchRevV, B, dB, v, n, tc, el,
  //			index, elIndex, tcIndex, grid, glm::mat3(Vector3d(-1.0f, 0.0f, 0.0f),
  //				Vector3d(0.0f, 1.0f, 0.0f),
  //				Vector3d(0.0f, 0.0f, 1.0f)), false);
  //	}

  //	// Patch reflected in y
  //	if (reflectY) {
  //		BuildPatch(patchRevV, B, dB, v, n, tc, el,
  //			index, elIndex, tcIndex, grid, glm::mat3(Vector3d(1.0f, 0.0f, 0.0f),
  //				Vector3d(0.0f, -1.0f, 0.0f),
  //				Vector3d(0.0f, 0.0f, 1.0f)), false);
  //	}

  //	// Patch reflected in x and y
  //	if (reflectX && reflectY) {
  //		BuildPatch(patch, B, dB, v, n, tc, el,
  //			index, elIndex, tcIndex, grid, glm::mat3(Vector3d(-1.0f, 0.0f, 0.0f),
  //				Vector3d(0.0f, -1.0f, 0.0f),
  //				Vector3d(0.0f, 0.0f, 1.0f)), true);
  //	}
  //}

  // void ShapeGenerator::BuildPatch(Vector3d patch[][4], float *B, float *dB, float *v, float *n,
  // float *tc, unsigned short *el, int &index, int &elIndex, int &tcIndex, int grid, glm::mat3
  // reflect, bool invertNormal) {
  //	int startIndex = index / 3;
  //	float tcFactor = 1.0f / grid;

  //	for (int i = 0; i <= grid; i++) {
  //		for (int j = 0; j <= grid; j++) {
  //			Vector3d pt = reflect * Evaluate(i, j, B, patch);
  //			Vector3d norm = reflect * EvaluateNormal(i, j, B, dB, patch);
  //			if (invertNormal)
  //				norm = -norm;

  //			v[index] = pt.x;
  //			v[index + 1] = pt.y;
  //			v[index + 2] = pt.z;

  //			n[index] = norm.x;
  //			n[index + 1] = norm.y;
  //			n[index + 2] = norm.z;

  //			tc[tcIndex] = i * tcFactor;
  //			tc[tcIndex + 1] = j * tcFactor;

  //			index += 3;
  //			tcIndex += 2;
  //		}
  //	}

  //	for (int i = 0; i < grid; i++)
  //	{
  //		int iStart = i * (grid + 1) + startIndex;
  //		int nextiStart = (i + 1) * (grid + 1) + startIndex;
  //		for (int j = 0; j < grid; j++)
  //		{
  //			el[elIndex] = iStart + j;
  //			el[elIndex + 1] = nextiStart + j + 1;
  //			el[elIndex + 2] = nextiStart + j;

  //			el[elIndex + 3] = iStart + j;
  //			el[elIndex + 4] = iStart + j + 1;
  //			el[elIndex + 5] = nextiStart + j + 1;

  //			elIndex += 6;
  //		}
  //	}
  //}

  // void ShapeGenerator::GetPatch(int patchNum, Vector3d patch[][4], bool reverseV) {
  //	for (int u = 0; u < 4; u++) {          // Loop in u direction
  //		for (int v = 0; v < 4; v++) {     // Loop in v direction
  //			if (reverseV) {
  //				patch[u][v] = Vector3d(
  //					cpdata[patchdata[patchNum][u * 4 + (3 - v)]][0],
  //					cpdata[patchdata[patchNum][u * 4 + (3 - v)]][1],
  //					cpdata[patchdata[patchNum][u * 4 + (3 - v)]][2]
  //				);
  //			}
  //			else {
  //				patch[u][v] = Vector3d(
  //					cpdata[patchdata[patchNum][u * 4 + v]][0],
  //					cpdata[patchdata[patchNum][u * 4 + v]][1],
  //					cpdata[patchdata[patchNum][u * 4 + v]][2]
  //				);
  //			}
  //		}
  //	}
  //}

  // void ShapeGenerator::ComputeBasisFunctions(float * B, float * dB, int grid) {
  //	float inc = 1.0f / grid;
  //	for (int i = 0; i <= grid; i++) {
  //		float t = i * inc;
  //		float tSqr = t * t;
  //		float oneMinusT = (1.0f - t);
  //		float oneMinusT2 = oneMinusT * oneMinusT;

  //		B[i * 4 + 0] = oneMinusT * oneMinusT2;
  //		B[i * 4 + 1] = 3.0f * oneMinusT2 * t;
  //		B[i * 4 + 2] = 3.0f * oneMinusT * tSqr;
  //		B[i * 4 + 3] = t * tSqr;

  //		dB[i * 4 + 0] = -3.0f * oneMinusT2;
  //		dB[i * 4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
  //		dB[i * 4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
  //		dB[i * 4 + 3] = 3.0f * tSqr;
  //	}
  //}

  // Vector3d ShapeGenerator::Evaluate(int gridU, int gridV, float *B, Vector3d patch[][4]) {
  //	Vector3d p(0.f, 0.f, 0.f);
  //	for (int i = 0; i < 4; i++) {
  //		for (int j = 0; j < 4; j++) {
  //			p += patch[i][j] * B[gridU * 4 + i] * B[gridV * 4 + j];
  //		}
  //	}
  //	return p;
  //}

  // Vector3d ShapeGenerator::EvaluateNormal(int gridU, int gridV, float *B, float *dB, Vector3d
  // patch[][4]) {
  //	Vector3d du(0.f, 0.f, 0.f);
  //	Vector3d dv(0.f, 0.f, 0.f);

  //	for (int i = 0; i < 4; i++) {
  //		for (int j = 0; j < 4; j++) {
  //			du += patch[i][j] * dB[gridU * 4 + i] * B[gridV * 4 + j];
  //			dv += patch[i][j] * B[gridU * 4 + i] * dB[gridV * 4 + j];
  //		}
  //	}
  //	return Vector3d::CrossProduct(du, dv).Normalize();
  //}

  Mesh* ShapeGenerator::GenerateKnots() {
    Mesh* mesh = new Mesh();
    return mesh;
  }
} // namespace Theodore
