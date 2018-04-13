#ifndef Color_h
#define Color_h

namespace Quark {
    class Color {
    public:
        Color();
        Color(float r, float g, float b, float a);
        
        //! Red, green, blue, alpha (translucency)
        float r, g, b, a;
        
        bool operator==(const Color& rhs);
        bool operator!=(const Color& rhs);
        float& operator[] (unsigned int i);
        
        static Color Lerp(const Color& a, const Color& b, float t);
        static unsigned char ConvertToByte(float value);
        static float ConvertToFloat(unsigned char value);
		static Color ConvertToColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        
        static Color white;
        static Color grey;
        static Color black;
        static Color red;
        static Color green;
        static Color blue;
        static Color yellow;
        static Color orange;
        static Color purple;
    };
}

#endif /* Color_h */
