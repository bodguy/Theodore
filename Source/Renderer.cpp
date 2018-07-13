#include "Renderer.h"
#include "VertexBuffer.h"
#include "Utility.h"

namespace Theodore {
	Renderer::Renderer(const std::string& name) : Component(name), mProgram(nullptr), mPrimitive(Primitive::Triangles), mIsVisibleGizmos(false) {
		mVao = new VertexArray();
		mVbos.clear();
		mEbos.clear();
		mNormalVisualizeProgram = Shader::Find("DebugNormal");
	}

	Renderer::~Renderer() {
		SafeContDealloc(mVbos);
		SafeContDealloc(mEbos);
		SafeDealloc(mVao);
		// DO NOT DEALLOCATE mProgram.
	}

	void Renderer::SetPrimitive(const Primitive primitive) {
		mPrimitive = primitive; 
	}

	void Renderer::SetVisibleGizmos(bool visible) {
		mIsVisibleGizmos = visible;
	}
}