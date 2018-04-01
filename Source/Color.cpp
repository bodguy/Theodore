#include "Color.h"
#include <algorithm> // for std::max, std::min
#include <cmath> // for std::floor

namespace Quark {
    Color::Color() :r(1.f), g(1.f), b(1.f), a(1.f) {
        
    }
    
    Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
        
    }
    
    bool Color::operator==(const Color& rhs) {
        return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a));
    }
    
    bool Color::operator!=(const Color& rhs) {
        return ((r != rhs.r) || (g != rhs.g) || (b != rhs.b) || (a != rhs.a));
    }
    
    float& Color::operator[] (unsigned int i) {
        switch (i) {
            case 0: return r;
            case 1: return g;
            case 2: return b;
            default: return a;
        }
    }
    
    Color Color::Lerp(const Color& a, const Color& b, float t) {
        // c = a+(b-a)*t
        return Color(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
    }
    
    unsigned char Color::ConvertToByte(float value) {
        float f2 = std::max(0.f, std::min(1.f, value));
        return static_cast<unsigned char>(std::floor(f2 == 1.0 ? 255 : f2 * 256));
    }
    
    float Color::ConvertToFloat(unsigned char value) {
        return value * (1.f / 255.f);
    }
    
    Color Color::white = Color(1.f, 1.f, 1.f, 1.f);
    Color Color::grey = Color(0.5f, 0.5f, 0.5f, 1.0f);
    Color Color::black = Color(0.f, 0.f, 0.f, 1.f);
    Color Color::red = Color(1.f, 0.f, 0.f, 1.f);
    Color Color::green = Color(0.f, 1.f, 0.f, 1.f);
    Color Color::blue = Color(0.f, 0.f, 1.f, 1.f);
    Color Color::yellow = Color(1.f, 1.f, 0.f, 1.f);
    Color Color::orange = Color(1.0f, 0.5f, 0.0f, 1.0f);
    Color Color::purple = Color(0.5f, 0.0f, 1.0f, 1.0f);
}
