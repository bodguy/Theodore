#ifndef Light_h
#define Light_h

#include "Enumeration.h"
#include "Color.h"
#include "Vector3d.h"

namespace Quark {
	class Light {
	public:
		Light();
		~Light();

		Vector3d mPosition;
		Vector3d mDirection;
		Color mColor;
		float mIntensity;
		float mConstant;
		float mLinear;
		float mQuadratic;
		float mCutOff;
		float mOuterCutOff;
		Enumeration::LightType mType;
	};
}

#endif /* Light_h */