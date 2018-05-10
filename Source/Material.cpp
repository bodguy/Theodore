#include "Material.h"

namespace Quark {
	Material::Material(Program* program) :mAmbient(), mDiffuse(), mSpecular(), mProgram(program) {

	}

	Material::~Material() {

	}
}