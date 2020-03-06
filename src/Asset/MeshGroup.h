// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef MeshScene_h
#define MeshScene_h

#include <vector>

namespace Theodore {
	class MeshChunk;
	class Material;
	class MeshGroup {
	public:
		MeshGroup();
		~MeshGroup();

		void AppendMesh(MeshChunk* mesh);
		void AppendMaterial(Material* material);
		size_t GetMeshCount() const { return meshes.size(); }
		size_t GetMaterialCount() const { return materials.size(); }

	private:
		std::vector<MeshChunk*> meshes;
		std::vector<Material*> materials;
	};
} // namespace Theodore

#endif /* MeshScene_h */
