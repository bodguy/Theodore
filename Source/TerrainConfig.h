#ifndef TerrainConfig_h
#define TerrainConfig_h

#include <string>

namespace Quark {
	class TerrainConfig {
	public:
		TerrainConfig();
		~TerrainConfig();

		void loadFile(const std::string& file);

	private:
		float mScaleY;
		float mScaleXZ;
		int* mLodRange;
		int* mLodMorphingArea;
	};
}

#endif // TerrainConfig_h