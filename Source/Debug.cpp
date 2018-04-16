#include "Debug.h"
#include "Matrix4x4.h"
#include "Vector4d.h"
#include "Vector3d.h"
#include "Vector2d.h"
#include "Color.h"
#include "Quaternion.h"
#include "Ray.h"
#include "Texture2D.h"
#include "Enumeration.h"
#include <stdarg.h> // for va_list
#include <stdio.h> // for vprintf

namespace Quark {
	int Debug::__tabCount = 0;
	std::string Debug::__tabString = "";
	bool Debug::__logSwitch = false;
    void Debug::Log(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

	void Debug::Log(const std::string& str) {
		Debug::Log("%s\n", str.c_str());
	}
    
	void Debug::Log(int i) {
		Debug::Log("%d\n", i);
	}

	void Debug::Log(float f) {
		Debug::Log("%f\n", f);
	}

	void Debug::Log(double d) {
		Debug::Log("%lf\n", d);
	}

    void Debug::Log(const Matrix4x4& matrix, unsigned int precision) {
        static const char* presition_table[7] = { "%.0f ", "%.1f ", "%.2f ", "%.3f ", "%.4f ", "%.5f ", "%.6f " };
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                Debug::Log(presition_table[precision], matrix.m44[i][j]);
            }
            Debug::Log("\n");
        }
    }
    
    void Debug::Log(const Vector4d& vector4) {
        Debug::Log("x: %f, y: %f, z: %f, w: %f\n", vector4.x, vector4.y, vector4.z, vector4.w);
    }
    
    void Debug::Log(const Vector3d& vector3) {
        Debug::Log("x: %f, y: %f, z: %f\n", vector3.x, vector3.y, vector3.z);
    }
    
    void Debug::Log(const Vector2d& vector2) {
        Debug::Log("x: %f, y: %f\n", vector2.x, vector2.y);
    }
    
    void Debug::Log(const Color& color) {
        Debug::Log("Color : R:%f[%d] G:%f[%d] B:%f[%d] A:%f[%d]\n", color.r, Color::ConvertToByte(color.r),
                   color.g, Color::ConvertToByte(color.g), color.b, Color::ConvertToByte(color.b), color.a, Color::ConvertToByte(color.a));
    }
    
    void Debug::Log(const Quaternion& quat) {
        Debug::Log("w: %f, x: %f, y: %f, z: %f\n", quat.w, quat.x, quat.y, quat.z);
    }
    
    void Debug::Log(const Ray& ray) {
        Debug::Log("Origin: (%f, %f, %f), Dir: (%f, %f, %f)\n",
                   ray.mOrigin.x, ray.mOrigin.y, ray.mOrigin.z, ray.mDirection.x, ray.mDirection.y, ray.mDirection.z);
    }
    
    void Debug::Log(const Texture* object) {
        Debug::Log("************************************\n");
        Debug::Log("Name: %s\n", object->mName.c_str());
        Debug::Log("TextureID: %d\n", object->mTextureID);
        Debug::Log("Width, Height: %d, %d\n", object->mWidth, object->mHeight);
        Debug::Log("Anisotrophy level: %f\n", object->mAnisoLevel);
        Debug::Log("Mipmap bias: %f\n", object->mMipMapBias);
        Debug::Log("Ref counts: %d\n", object->mRefCount);
        
        Debug::Log("Texture Filter mode: ");
        switch (object->mFilterMode) {
            case Enumeration::Nearest:
                Debug::Log("Nearest\n");
                break;
            case Enumeration::Bilinear:
                Debug::Log("Bilinear\n");
                break;
            case Enumeration::Trilinear:
                Debug::Log("Trilinear\n");
                break;
        }
        
        Debug::Log("Texture Wrap mode: ");
        switch (object->mWrapMode) {
            case Enumeration::Repeat:
                Debug::Log("Repeat\n");
                break;
            case Enumeration::Clamp:
                Debug::Log("Clamp\n");
                break;
            case Enumeration::ClampEdge:
                Debug::Log("ClampEdge\n");
                break;
        }
        
        Debug::Log("Color Key Value: ");
        Debug::Log(object->mColorKey);
    }

	void Debug::LogOnOff(bool b) {
		Debug::__logSwitch = b;
	}

	bool Debug::GetLogSwitch() {
		return Debug::__logSwitch;
	}
}
