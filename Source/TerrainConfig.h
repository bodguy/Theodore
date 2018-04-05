#ifndef TerrainConfig_h
#define TerrainConfig_h

#include <string>
#include <vector>

namespace Quark {
	class TerrainConfig {
	public:
		TerrainConfig();
		~TerrainConfig();

		void LoadFile(const std::string& file);

		float GetScaleY() const;
		float GetScaleXZ() const;

	private:
		int GetMorphingArea(int lod);
		void SetLodRange(int index, int lod_range);

		float mScaleY;
		float mScaleXZ;
		int* mLodRange;
		int* mLodMorphingArea;
	};
}

#endif // TerrainConfig_h