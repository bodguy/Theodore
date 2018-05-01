#ifndef MeshRenderer_h
#define MeshRenderer_h

#include "Component.h"

namespace Quark {
	class Material;
	class Mesh;
	class Program;
	class MeshRenderer : public Component {
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		void SetMaterial(Material* mat);

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Material* mMaterial;
		Mesh* mMeshCache;
		Program* mProgram;
	};
}

#endif /* MeshRenderer_h */