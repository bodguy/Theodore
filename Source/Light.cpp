#include "Light.h"
#include "Debug.h"
#include "GameObject.h"
#include "Math.h"
#include "ShadowInfo.h"
#include "Utility.h"

namespace Theodore {
  const unsigned int Light::MaxLightCount = 50;
  Light::Light(LightType type)
      : Component("Light"), type(type), ambient(Color(0.01f, 0.01f, 0.01f, 0.01f)),
        diffuse(Color(0.01f, 0.01f, 0.01f, 0.01f)), specular(Color(0.1f, 0.1f, 0.1f, 0.1f)),
        constant(0.f), linear(0.f), quadratic(0.f), cutOff(0.f), outerCutOff(0.f) {
    switch (type) {
    case LightType::DirectionalLight:
      break;
    case LightType::PointLight:
      constant = 1.f;
      linear = 0.09f;
      quadratic = 0.032f;
      break;
    case LightType::SpotLight:
      cutOff = Math::Cos(Math::Radians(12.5f));
      outerCutOff = Math::Cos(Math::Radians(15.0f));
      constant = 1.f;
      linear = 0.09f;
      quadratic = 0.032f;
      break;
    }
    mShadowInfo = new ShadowInfo();
  }

  Light::~Light() { SafeDealloc(mShadowInfo); }

  void Light::Update(float deltaTime) {}

  void Light::Render() {}

  bool Light::CompareEquality(const Object& rhs) const { return false; }

  bool Light::Destroy() { return false; }
}
