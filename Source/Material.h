#ifndef Material_h
#define Material_h

#include "Color.h"
#include <vector>

namespace Quark {
	class Program; class Texture;
	class Material {
	public:
		Material(Program* program);
		Material(Color ambient, Color diffse, Color specular, float shininess);
		~Material();

		Program* shader;
		Texture* texture0;
		Texture* texture1;
		Texture* texture2;
		Texture* texture3;
		Texture* texture4;
		Color ambient;
		Color diffuse;
		Color specular;
		float shininess;

		static const Material emerald;
		static const Material jade;
		static const Material obsidian;
		static const Material pearl;
		static const Material ruby;
		static const Material turquoise;
		static const Material brass;
		static const Material bronze;
		static const Material chrome;
		static const Material copper;
		static const Material gold;
		static const Material silver;
		static const Material black_plastic;
		static const Material black_rubber;
	};
}

#endif /* Material_h */