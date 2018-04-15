#include "Terrain.h"
#include "TerrainConfig.h"
#include "TerrainQuadtree.h"
#include "Utility.h"

namespace Quark {
	Terrain::Terrain() :mConfig(NULL) {

	}

	Terrain::~Terrain() {
		SafeDealloc(mConfig);
	}

	void Terrain::init(const std::string& fileName) {
		mConfig = new TerrainConfig();
		mConfig->LoadFile(fileName);

		AddChild(new TerrainQuadtree(mConfig));
	}

	void Terrain::UpdateQuadtree() {
		// 카메라 움직였을때 로직 들어가야함.
		std::list<Node*>::iterator iter = GetChildren().begin();
		static_cast<TerrainQuadtree*>(*iter)->UpdateQuadtree();
	}
}