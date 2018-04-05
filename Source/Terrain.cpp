#include "Terrain.h"
#include "TerrainConfig.h"
#include "TerrainQuadtree.h"

namespace Quark {
	Terrain::Terrain() {

	}

	Terrain::~Terrain() {

	}

	void Terrain::init(const std::string& file) {
		mConfig = new TerrainConfig();
		mConfig->LoadFile(file);

		AddChild(new TerrainQuadtree(mConfig));
	}
}