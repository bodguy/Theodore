#include "TerrainConfig.h"
#include "File.h"
#include "Enumeration.h"

namespace Quark {
	TerrainConfig::TerrainConfig() {
	}

	TerrainConfig::~TerrainConfig() {
	}

	void TerrainConfig::loadFile(const std::string& file) {
		std::string line = "";
		File buffer;
		buffer.Open(file, Enumeration::OpenMode::Read);

		while (buffer.Validate()) {
			line = buffer.GetLine();
			// 6:47
		}
	}
}