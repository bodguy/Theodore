#ifndef Material_h
#define Material_h

#include "Color.h"

namespace Quark {
	class Material {
	public:
		Material();
		~Material();

		Color GetAmbient() const { return mAmbient; }
		Color GetDiffuse() const { return mDiffuse; }
		Color GetSpecular() const { return mSpecular; }

	private:
		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;
	};
}

#endif /* Material_h */