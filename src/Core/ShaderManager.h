// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef ShaderManager_h
#define ShaderManager_h

#include <string>
#include <map>

namespace Theodore {
	class Pipeline;
	class ShaderManager {
		friend class Shader;

	public:
		ShaderManager();
		~ShaderManager();

		static bool Append(Pipeline* program);

	private:
		static ShaderManager* shaderManager;
		std::map<std::string, Pipeline*> loadedPiplineLookUpTable;
	};
}

#endif /* ShaderManager_h */
