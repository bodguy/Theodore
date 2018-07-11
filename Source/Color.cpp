#include "Color.h"
#include "Math.h"
#include <algorithm> // for std::max, std::min
#include <cmath> // for std::floor

namespace Quark {
    Color::Color() :r(1.f), g(1.f), b(1.f), a(1.f) {
        
    }
    
    Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
        
    }
    
    bool Color::operator==(const Color& rhs) {
        return (Math::IsEqual(r, rhs.r) && Math::IsEqual(g, rhs.g) && Math::IsEqual(b, rhs.b) && Math::IsEqual(a, rhs.a));
    }
    
    bool Color::operator!=(const Color& rhs) {
        return !(*this == rhs);
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
        return Color(std::max(0.f, std::min(1.f, a.r + (b.r - a.r) * t)), 
			std::max(0.f, std::min(1.f, a.g + (b.g - a.g) * t)), 
			std::max(0.f, std::min(1.f, a.b + (b.b - a.b) * t)), 
			std::max(0.f, std::min(1.f, a.a + (b.a - a.a) * t)));
    }
    
    unsigned char Color::ConvertToByte(float value) {
        float f2 = std::max(0.f, std::min(1.f, value));
        return static_cast<unsigned char>(std::floor(f2 == 1.0 ? 255 : f2 * 256));
    }
    
	Color Color::RGBToColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		float invConverter = 1.f / 255.f;
		return Color(r * invConverter, g * invConverter, b * invConverter, a * invConverter);
	}
   
	Color Color::HexToColor(unsigned int hexValue) {
		return Color(((hexValue >> 16) & 0xFF) * (1.f / 255.f), // Extract the R byte
					 ((hexValue >> 8) & 0xFF) * (1.f / 255.f), // Extract the G byte
					 ((hexValue) & 0xFF) * (1.f / 255.f), // Extract the B byte
					 ((hexValue >> 24) & 0xFF) * (1.f / 255.f)); // Extract the A byte
	}

	Color Color::HexToColor(const std::string& hexString) {
		int hexValue = std::stoi(hexString.substr(1, hexString.size() - 1), 0, 16);
		return Color(((hexValue >> 16) & 0xFF) * (1.f / 255.f), // Extract the R byte
					((hexValue >> 8) & 0xFF) * (1.f / 255.f), // Extract the G byte
					((hexValue) & 0xFF) * (1.f / 255.f), // Extract the B byte
					1.f); // alpha is fixed to 1
	}

	Color Color::CMKYToColor(float c, float m, float y, float k) {
		float cmyk_scale = 1.f / 100.f;
		float k2 = std::min(100.f, k);
		float precomputed = 1.f - k2 * cmyk_scale;

		return Color((1.f - std::min(100.f, c) * cmyk_scale) * precomputed,
					 (1.f - std::min(100.f, m) * cmyk_scale) * precomputed,
					 (1.f - std::min(100.f, y) * cmyk_scale) * precomputed,
					 1.f);
	}

	Color Color::HSVToColor(float h, float s, float v) {
		// TODO
		return Color();
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
	Color Color::GizmoRed = Color(0.65f, 0.2f, 0.11f, 1.0f);
	Color Color::GizmoGreen = Color(0.47f, 0.8f, 0.29f, 1.0f);
	Color Color::GizmoBlue = Color(0.2f, 0.39f, 0.84f, 1.0f);
}
