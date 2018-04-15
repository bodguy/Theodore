#ifndef TerrainQuadtree_h
#define TerrainQuadtree_h

#include "Node.h"

namespace Quark {
	class TerrainConfig; class InputStream;
	class TerrainQuadtree : public Node {
	public:
		TerrainQuadtree(TerrainConfig* config);
		~TerrainQuadtree();

		static int GetRootNodes();
		void GenerateVertexData(InputStream& stream);
		virtual void UpdateQuadtree() override;


	private:
		static int rootNodes;
	};
}


#endif // TerrainQuadtree_h