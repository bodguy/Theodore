#ifndef MeshRenderer_h
#define MeshRenderer_h

#include "Renderer.h"

namespace Quark {
	class Material; class Mesh; class Pipeline;
	class MeshRenderer : public Renderer {
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		void SetMaterial(Material* mat);
		void SetMesh(Mesh* mesh);
		Mesh* GetMesh() const;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

		void InternalRender();

	protected:
		Material* mMaterial;
		Mesh* mMesh;
#ifdef _DEBUG
		Pipeline* DEBUG_PROGRAM;
#endif
	};
}

#endif /* MeshRenderer_h */