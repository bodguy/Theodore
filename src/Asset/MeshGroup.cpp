// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "MeshGroup.h"
#include "Component/Material.h"
#include "Helper/Utility.h"
#include "WaveFrontObjMesh.h"

namespace Theodore {
	MeshGroup::MeshGroup()  : meshes(), materials() {
		meshes.clear();
		materials.clear();
	}

	MeshGroup::~MeshGroup() {
		SafeContDealloc(meshes);
		SafeContDealloc(materials);
	}

	void MeshGroup::AppendMesh(MeshChunk* mesh) {
		meshes.emplace_back(mesh);
	}

	void MeshGroup::AppendMaterial(Material* material) {
		materials.emplace_back(material);
	}
} // namespace Theodore
