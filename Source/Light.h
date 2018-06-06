#ifndef Light_h
#define Light_h

#include "Enumeration.h"
#include "Color.h"
#include "Component.h"

namespace Quark {
	class Transform; class ShadowInfo;
	class Light : public Component {
	public:
		Light(LightType type);
		virtual ~Light();

		Transform* GetTransform() const;

		LightType type;
		Color ambient;
		Color diffuse;
		Color specular;

		// point light attenuation
		float constant;
		float linear;
		float quadratic;

		// spot light theta, phi
		float cutOff;
		float outerCutOff;

		static unsigned int MaxLightCount;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

		Transform* mTransform;
		ShadowInfo* mShadowInfo;
	};
}

#endif /* Light_h */