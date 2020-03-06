// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Color_h
#define Color_h

#include <string>

namespace Theodore {
	class Vector3d;
	class Vector4d;
  class Color {
  public:
    Color();
		Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const Vector3d& other);
		Color(const Vector4d& other);

    //! Red, green, blue, alpha (translucency)
    float r, g, b, a;

    bool operator==(const Color& rhs);
    bool operator!=(const Color& rhs);
    float& operator[](unsigned int i);

    static Color Lerp(const Color& a, const Color& b, float t);
    static unsigned char ToByte(float value);
    static Color FromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    static Color FromHex(unsigned int hexValue);
    static Color FromHex(const std::string& hexString);
    static Color FromCMYK(float c, float m, float y, float k);
    static Color FromHSV(int hueInDegree, float saturation, float value);

    static Color white;
    static Color grey;
    static Color black;
    static Color red;
    static Color green;
    static Color blue;
    static Color yellow;
    static Color orange;
    static Color purple;
    static Color GizmoRed;
    static Color GizmoGreen;
    static Color GizmoBlue;
  };
}  // namespace Theodore

#endif /* Color_h */
