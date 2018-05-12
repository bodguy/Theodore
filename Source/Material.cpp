#include "Material.h"

namespace Quark {
	Material::Material(Program* program) :mAmbient(), mDiffuse(), mSpecular(), mShininess(32.f), mProgram(program) {
		mTextures.clear();
	}

	Material::~Material() {

	}
}