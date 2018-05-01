#include "MeshFilter.h"
#include "Utility.h"

namespace Quark {
	MeshFilter::MeshFilter() : Component("MeshFilter"), mMesh(nullptr) {

	}

	MeshFilter::~MeshFilter() {
		SafeDealloc(mMesh);
	}

	void MeshFilter::SetMesh(Mesh* mesh) {
		mMesh = mesh;
	}

	Mesh* MeshFilter::GetMesh() const {
		return mMesh;
	}

	void MeshFilter::Update(double deltaTime) {
	}

	void MeshFilter::Render() {
		return;
	}

	bool MeshFilter::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool MeshFilter::Destroy() {
		return false;
	}
}