#include "Renderer.h"
#include "VertexBuffer.h"
#include "Utility.h"

namespace Quark {
	Renderer::Renderer(const std::string& name) : Component(name), mProgram(nullptr) {
		mVao = new VertexArray();
		mVbos.clear();
		mEbos.clear();
	}

	Renderer::~Renderer() {
		SafeContDealloc(mVbos);
		SafeContDealloc(mEbos);
		SafeDealloc(mVao);
		// DO NOT DEALLOCATE mProgram.
	}
}