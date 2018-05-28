#include "Material.h"

namespace Quark {
	Material::Material(Program* program) :ambient(), diffuse(), specular(), shininess(64.f), shader(program),
		texture0(nullptr), texture1(nullptr), texture2(nullptr), texture3(nullptr), texture4(nullptr), renderTexture(nullptr) {
	
	}

	Material::Material(Color ambient, Color diffse, Color specular, float shininess) : ambient(ambient), diffuse(diffse), specular(specular), shininess(shininess), shader(nullptr),
		texture0(nullptr), texture1(nullptr), texture2(nullptr), texture3(nullptr), texture4(nullptr), renderTexture(nullptr) {

	}

	Material::~Material() {

	}

	const Material Material::emerald = Material(Color(0.0215f, 0.1745f, 0.0215f, 1.f), Color(0.07568f, 0.61424f, 0.07568f, 1.f), Color(0.633f, 0.727811f, 0.633f, 1.f), 0.6f);
	const Material Material::jade = Material(Color(0.135f, 0.222f, 0.1575f, 1.f), Color(0.54f, 0.89f, 0.63f, 1.f), Color(0.316228f, 0.316228f, 0.316228f, 1.f), 0.1f);
	const Material Material::obsidian = Material(Color(0.05375f, 0.05f, 0.06625f, 1.f), Color(0.118275f, 0.17f, 0.22525f, 1.f), Color(0.332741f, 0.328634f, 0.346435f, 1.f), 0.3f);
	const Material Material::pearl = Material(Color(0.25f, 0.20725f, 0.20725f, 1.f), Color(1.f, 0.829f, 0.829f, 1.f), Color(0.296648f, 0.296648f, 0.296648f, 1.f), 0.088f);
	const Material Material::ruby = Material(Color(0.1745f, 0.01175f, 0.01175f, 1.f), Color(0.61424f, 0.04136f, 0.04136f, 1.f), Color(0.727811f, 0.626959f, 0.626959f, 1.f), 0.6f);
	const Material Material::turquoise = Material(Color(0.1f, 0.18725f, 0.1745f, 1.f), Color(0.396f, 0.74151f, 0.69102f, 1.f), Color(0.297254f, 0.30829f, 0.306678f, 1.f), 0.1f);
	const Material Material::brass = Material(Color(0.329412f, 0.223529f, 0.027451f, 1.f), Color(0.780392f, 0.568627f, 0.113725f, 1.f), Color(0.992157f, 0.941176f, 0.807843f, 1.f), 0.21794872f);
	const Material Material::bronze = Material(Color(0.2125f, 0.1275f, 0.054f, 1.f), Color(0.714f, 0.4284f, 0.18144f, 1.f), Color(0.393548f, 0.271906f, 0.166721f, 1.f), 0.2f);
	const Material Material::chrome = Material(Color(0.25f, 0.25f, 0.25f, 1.f), Color(0.4f, 0.4f, 0.4f, 1.f), Color(0.774597f, 0.774597f, 0.774597f, 1.f), 0.6f);
	const Material Material::copper = Material(Color(0.19125f, 0.0735f, 0.0225f, 1.f), Color(0.7038f, 0.27048f, 0.0828f, 1.f), Color(0.256777f, 0.137622f, 0.086014f, 1.f), 0.1f);
	const Material Material::gold = Material(Color(0.24725f, 0.1995f, 0.0745f, 1.f), Color(0.75164f, 0.60648f, 0.22648f, 1.f), Color(0.628281f, 0.555802f, 0.366065f, 1.f), 0.4f);
	const Material Material::silver = Material(Color(0.19225f, 0.19225f, 0.19225f, 1.f), Color(0.50754f, 0.50754f, 0.50754f, 1.f), Color(0.508273f, 0.508273f, 0.508273f, 1.f), 0.4f);
}