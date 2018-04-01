#ifndef Material_h
#define Material_h

#include "Color.h"

namespace Quark {
	class Material {
	public:
		Material();
		~Material();

	private:
		Color mAmbient;
		Color mDiffuse;
		Color mSpecular;
	};
}

#endif /* Material_h */