#include "TerrainQuadtree.h"
#include "TerrainNode.h"
#include "Transform.h"
#include "TerrainConfig.h"
#include "VertexBuffer.h"
#include "Vector2d.h"

namespace Quark {
	int TerrainQuadtree::rootNodes = 8;
	TerrainQuadtree::TerrainQuadtree(TerrainConfig* config) {
		for (int i = 0; i < TerrainQuadtree::rootNodes; i++) {
			for (int j = 0; j < TerrainQuadtree::rootNodes; j++) {
				AddChild(new TerrainNode());
			}
		}

		GetWorldTransform()->SetScale(Vector3d(config->GetScaleXZ(), config->GetScaleY(), config->GetScaleXZ()));
		GetWorldTransform()->SetPosition(Vector3d(-config->GetScaleXZ()/2.f, 0.f, -config->GetScaleXZ() / 2.f));
	}

	TerrainQuadtree::~TerrainQuadtree() {
	}

	int TerrainQuadtree::GetRootNodes() {
		return TerrainQuadtree::rootNodes;
	}

	InputStream* TerrainQuadtree::GenerateVertexData() {
		float terrainVerts[32] = {
			0.f, 0.f,
			0.333f, 0.f,
			0.666f, 0.f,
			1.f, 0.f,
			0.f, 0.333f,
			0.333f, 0.333f,
			0.666f, 0.333f,
			1.f, 0.333f,
			0.f, 0.666f,
			0.333f, 0.666f,
			0.666f, 0.666f,
			1.f, 0.666f,
			0.f, 1.f,
			0.333f, 1.f,
			0.666f, 1.f,
			1.f, 1.f
		};

		InputStream* vertices = new InputStream();
		int stride = 2;
		for (int i = 0; i < 16; i++) {
			vertices->Vec2(Vector2d(terrainVerts[i * stride], terrainVerts[i * stride + 1]));
		}

		return vertices; //18:10
	}
}