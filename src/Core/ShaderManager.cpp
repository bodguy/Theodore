// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "ShaderManager.h"
#include "Helper/Utility.h"
#include "Asset/Shader.h"

namespace Theodore {
	ShaderManager *ShaderManager::shaderManager = nullptr;

	ShaderManager::ShaderManager() {
		shaderManager = this;
		shaderManager->loadedPiplineLookUpTable.clear();
	}

	ShaderManager::~ShaderManager() {
		for (auto &i : loadedPiplineLookUpTable) {
			SafeDealloc(i.second);
		}
		loadedPiplineLookUpTable.clear();
	}

	bool ShaderManager::Append(Pipeline *program) {
		std::pair<std::map<std::string, Pipeline *>::iterator, bool> result;
		result = shaderManager->loadedPiplineLookUpTable.insert(std::pair<std::string, Pipeline *>(program->name, program));
		return result.second;
	}
}