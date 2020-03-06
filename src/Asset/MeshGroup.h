// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef MeshScene_h
#define MeshScene_h

#include <vector>
#include "Asset/Mesh.h"
#include "Component/Material.h"

namespace Theodore {
	class MeshGroup {
	public:
		MeshGroup();

		std::vector<Mesh> mMeshes;
		std::vector<Material> mMaterials;
	};
} // namespace Theodore

#endif /* MeshScene_h */
