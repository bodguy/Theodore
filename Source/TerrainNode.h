#ifndef TerrainNode_h
#define TerrainNode_h

#include "Node.h"
#include "VertexBuffer.h"
#include "Vector3d.h"
#include "Vector2d.h"

namespace Quark {
	class TerrainConfig;
	class TerrainNode : public Node {
	public:
		TerrainNode(VertexArray& vao, TerrainConfig* config, Vector2d& location, int lod, Vector2d& index);
		~TerrainNode();

		void Render();
		virtual void UpdateQuadtree() override;
		void UpdateChildNodes();

	private:
		VertexArray mVao;
		bool mIsLeaf;
		Vector2d mIndex;
		int mLod;
		Vector2d mLocation;
		TerrainConfig* mConfig;
		float mGap;
		Vector3d mWordPosition;
	};
}

#endif // TerrainNode_h