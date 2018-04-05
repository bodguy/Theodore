#include "TerrainNode.h"
#include "TerrainConfig.h"
#include "TerrainQuadtree.h"
#include "Transform.h"
#include "Graphics.h"
#include "Enumeration.h"

namespace Quark {
	TerrainNode::TerrainNode(VertexArray& vao, TerrainConfig* config, Vector2d& location, int lod, Vector2d& index) 
		:mVao(vao), mIsLeaf(true), mIndex(index), mLod(lod), mLocation(location), mConfig(config){
		mGap = 1.f / (TerrainQuadtree::GetRootNodes() * static_cast<float>(std::pow(2, lod)));

		GetLocalTransform()->SetScale(Vector3d(mGap, 0.f, mGap));
		GetLocalTransform()->SetPosition(Vector3d(mLocation.x, 0.f, mLocation.y));

		GetWorldTransform()->SetScale(Vector3d(config->GetScaleXZ(), config->GetScaleY(), config->GetScaleXZ()));
		GetWorldTransform()->SetPosition(Vector3d(-config->GetScaleXZ() / 2.f, 0.f, -config->GetScaleXZ() / 2.f));
	}

	TerrainNode::~TerrainNode() {
	}

	void TerrainNode::Render() {
		if (mIsLeaf) {
			Graphics::DrawArrays(mVao, Enumeration::Primitive::Patches, 0, 16);
		}
	}

	void TerrainNode::Update() {
		for (Node* node : *GetChildren()) {
			dynamic_cast<TerrainNode*>(node)->Update();
		}
	}
}