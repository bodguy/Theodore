#include "TerrainQuadtree.h"
#include "TerrainNode.h"
#include "Transform.h"
#include "TerrainConfig.h"
#include "VertexBuffer.h"
#include "Vector2d.h"
#include "Enumeration.h"
#include "Graphics.h"

namespace Quark {
	int TerrainQuadtree::rootNodes = 8;
	TerrainQuadtree::TerrainQuadtree(TerrainConfig* config) {
		InputStream data;
		GenerateVertexData(data);
		
		Buffer buffer(Enumeration::BufferType::BufferVertex);
		buffer.Data(data.Pointer(), data.Size(), Enumeration::BufferUsage::StaticDraw);

		VertexArray vao;
		vao.BindAttribute(0, buffer, 2, sizeof(Vector2d), NULL);
		Graphics::PatchParameter(Enumeration::PatchParameters::PatchVertices, 3);

		for (int i = 0; i < TerrainQuadtree::rootNodes; i++) {
			for (int j = 0; j < TerrainQuadtree::rootNodes; j++) {
				AddChild(new TerrainNode(vao, config, 
					Vector2d(i / static_cast<float>(rootNodes), j /static_cast<float>(rootNodes)), 0, Vector2d(static_cast<float>(i), static_cast<float>(j))));
			}
		}

		GetWorldTransform().SetScale(Vector3d(config->GetScaleXZ(), config->GetScaleY(), config->GetScaleXZ()));
		GetWorldTransform().SetPosition(Vector3d(-config->GetScaleXZ()/2.f, 0.f, -config->GetScaleXZ() / 2.f));
	}

	TerrainQuadtree::~TerrainQuadtree() {
	}

	int TerrainQuadtree::GetRootNodes() {
		return TerrainQuadtree::rootNodes;
	}

	void TerrainQuadtree::GenerateVertexData(InputStream& stream) {
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

		int stride = 2;
		for (int i = 0; i < 16; i++) {
			stream.Vec2(Vector2d(terrainVerts[i * stride], terrainVerts[i * stride + 1]));
		}
	}

	void TerrainQuadtree::UpdateQuadtree() {
		for (Node* child : GetChildren()) {
			static_cast<TerrainNode*>(child)->UpdateQuadtree();
		}
	}
}