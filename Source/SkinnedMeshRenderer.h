#ifndef SkinnedMeshRenderer_h
#define SkinnedMeshRenderer_h

#include "Component.h"

namespace Quark {
	class SkinnedMeshRenderer : public Component {
	public:
		SkinnedMeshRenderer();
		virtual ~SkinnedMeshRenderer();

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;
	};
}

#endif /* SkinnedMeshRenderer_h */