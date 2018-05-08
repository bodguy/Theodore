#include "SkinnedMeshRenderer.h"

namespace Quark {
	SkinnedMeshRenderer::SkinnedMeshRenderer() : Component("SkinnedMeshRenderer") {
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer() {
	}

	void SkinnedMeshRenderer::Update(double deltaTime) {
	}

	void SkinnedMeshRenderer::Render() {

	}

	bool SkinnedMeshRenderer::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool SkinnedMeshRenderer::Destroy() {
		return false;
	}
}