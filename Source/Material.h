#ifndef Material_h
#define Material_h

#include "Color.h"
#include <map>

namespace Quark {
	class Program; class Texture;
	class Material {
	public:
		Material(Program* program);
		Material(Color ambient, Color diffse, Color specular, float shininess);
		~Material();

		Program* GetProgram() const { return mProgram; }
		void SetProgram(Program* program) { mProgram = program; }

	public:
		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;
		float mShininess;

		static const Material emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze, chrome, copper, gold, silver, black_plastic, black_rubber;

	private:
		Program* mProgram;
		std::map<std::string, Texture*> mTextures;
	};
}

#endif /* Material_h */