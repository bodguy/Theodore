#ifndef MeshFilter_h
#define MeshFilter_h

#include "Component.h"

namespace Quark {
	class Mesh;
	class MeshFilter : public Component {
	public:
		MeshFilter();
		virtual ~MeshFilter();

		void SetMesh(Mesh* mesh);
		Mesh* GetMesh() const;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Mesh* mMesh;
	};
}

#endif /* MeshFilter_h */