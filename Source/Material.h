#ifndef Material_h
#define Material_h

#include "Color.h"
#include <map>

namespace Quark {
	class Program; class Texture;
	class Material {
	public:
		Material(Program* program);
		~Material();

		Color GetAmbient() const { return mAmbient; }
		Color GetDiffuse() const { return mDiffuse; }
		Color GetSpecular() const { return mSpecular; }
		float GetShininess() const { return mShininess; }
		Program* GetProgram() const { return mProgram; }

	private:
		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;
		float mShininess;
		Program* mProgram;
		std::map<std::string, Texture*> mTextures;
	};
}

#endif /* Material_h */