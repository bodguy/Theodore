// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Light_h
#define Light_h

#include "Component.h"
#include "Graphics/Enumeration.h"
#include "Math/Color.h"

namespace Theodore {
  class Transform;
  class ShadowInfo;
  class Light : public Component {
  public:
    Light(LightType type);
    virtual ~Light();

  public:
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

    static const unsigned int MaxLightCount;

  private:
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual bool CompareEquality(const Object& rhs) const override;
    virtual bool Destroy() override;

  private:
    ShadowInfo* mShadowInfo;
  };
}  // namespace Theodore

#endif /* Light_h */