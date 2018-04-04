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
		bool IsMatching(const std::string& txt, const std::string& regex);
		bool IsComment(const std::string& c);
		unsigned int Split(const std::string& txt, std::vector<std::string>& strs, char ch);
		int GetMorphingArea(int lod);
		void SetLodRange(int index, int lod_range);

		float mScaleY;
		float mScaleXZ;
		int* mLodRange;
		int* mLodMorphingArea;
	};
}

#endif // TerrainConfig_h