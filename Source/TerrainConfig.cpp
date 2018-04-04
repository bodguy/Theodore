#include "TerrainConfig.h"
#include "File.h"
#include "Enumeration.h"
#include "TerrainQuadtree.h"
#include <algorithm>
#include <regex>

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

		while (buffer.Validate()) {
			line = buffer.GetLine();
			if (!IsComment(line)) {
				std::vector<std::string> tokens;
				Split(line, tokens, ' ');

				if (tokens.size() == 0) {
					continue;
				} else if (tokens[0] == "scaleY") {
					mScaleY = std::stof(tokens[1]);
				} else if (tokens[0] == "scaleXZ") {
					mScaleXZ = std::stof(tokens[1]);
				} else if (IsMatching(tokens[0], "lod[1-9]_range")) {
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

	float TerrainConfig::GetScaleY() const {
		return mScaleY;
	}
	
	float TerrainConfig::GetScaleXZ() const {
		return mScaleXZ;
	}

	bool TerrainConfig::IsMatching(const std::string& txt, const std::string& regex) {
		std::regex pattern(regex);
		if (std::regex_match(txt, pattern)) {
			return true;
		}

		return false;
	}

	bool TerrainConfig::IsComment(const std::string& c) {
		return (c.empty()) || (c.at(0) == '#');
	}

	unsigned int TerrainConfig::Split(const std::string& txt, std::vector<std::string>& strs, char ch) {
		size_t pos = txt.find(ch);
		size_t initialPos = 0;
		strs.clear();

		// Decompose statement
		while (pos != std::string::npos) {
			strs.push_back(txt.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = txt.find(ch, initialPos);
		}

		// Add the last one
		strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

		return strs.size();
	}

	int TerrainConfig::GetMorphingArea(int lod) {
		return static_cast<int>((mScaleXZ / TerrainQuadtree::GetRootNodes()) / std::pow(2, lod));
	}
	
	void TerrainConfig::SetLodRange(int index, int lod_range) {
		mLodRange[index] = lod_range;
		mLodMorphingArea[index] = lod_range - GetMorphingArea(index + 1);
	}
}