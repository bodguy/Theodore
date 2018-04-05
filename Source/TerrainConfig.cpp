#include "TerrainConfig.h"
#include "File.h"
#include "Enumeration.h"
#include "TerrainQuadtree.h"
#include "StringUtil.h"
#include <algorithm>

namespace Quark {
	TerrainConfig::TerrainConfig() :mScaleY(0.f), mScaleXZ(0.f), mLodRange(new int[8]), mLodMorphingArea(new int[8]){
	}

	TerrainConfig::~TerrainConfig() {
		delete[] mLodRange;
		delete[] mLodMorphingArea;
	}

	void TerrainConfig::LoadFile(const std::string& file) {
		std::string line = "";
		File buffer;
		buffer.Open(file, Enumeration::OpenMode::Read);

		if(buffer.IsOpen()) {
			while (buffer.Validate()) {
				line = buffer.GetLine();
				if (!StringUtil::Comment(line)) {
					std::vector<std::string> tokens;
					size_t size = StringUtil::Split(line, tokens, ' ');

					if(size != 0) {
						if (tokens[0] == "scaleY") {
							mScaleY = std::stof(tokens[1]);
						} else if (tokens[0] == "scaleXZ") {
							mScaleXZ = std::stof(tokens[1]);
						} else if (StringUtil::PatternMatch(tokens[0], "lod[1-9]_range")) {
							int i = std::stoi(tokens[0].substr(3, 1)) - 1;
							int value = std::stoi(tokens[1]);
							if (value == 0) {
								mLodRange[i] = 0;
								mLodMorphingArea[i] = 0;
							} else {
								SetLodRange(i, value);
							}
						}
					}

				}
			}
			buffer.Close();
		}

	}

	float TerrainConfig::GetScaleY() const {
		return mScaleY;
	}
	
	float TerrainConfig::GetScaleXZ() const {
		return mScaleXZ;
	}

	int TerrainConfig::GetMorphingArea(int lod) {
		return static_cast<int>((mScaleXZ / TerrainQuadtree::GetRootNodes()) / std::pow(2, lod));
	}
	
	void TerrainConfig::SetLodRange(int index, int lod_range) {
		mLodRange[index] = lod_range;
		mLodMorphingArea[index] = lod_range - GetMorphingArea(index + 1);
	}
}