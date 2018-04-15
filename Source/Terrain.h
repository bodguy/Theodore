#ifndef Terrain_h
#define Terrain_h

#include "Node.h"

namespace Quark {
	class TerrainConfig;
	class Terrain : public Node {
	public:
		Terrain();
		~Terrain();

		void init(const std::string& fileName);
		void UpdateQuadtree();

	private:
		TerrainConfig* mConfig;
	};
}

#endif // Terrain_h