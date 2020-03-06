// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Texture_h
#define Texture_h

#include "Asset.h"
#include "Graphics/Enumeration.h"
#include "Math/Color.h"
#include "Platform/CondiitonalVars.h"
#include "Math/Vector3d.h"
#include GLEW_INCLUDE_DIR
#include <vector>

namespace Theodore {
	struct TextureOption {
		TextureOption()
			: clamp(false), blendu(true), blendv(true), bump_multiplier(1.f), sharpness(1.f),
			 brightness(0.f), contrast(1.f), origin_offset(), scale(1.f, 1.f, 1.f), turbulence(), imfchan('m') {}

		bool clamp; // -clamp (default false)
		bool blendu; // -blendu (default true)
		bool blendv; // -blendv (default true)
		float bump_multiplier; // -bm (for bump maps only, default 1.0)
		float sharpness; // -boost (default 1.0)
		float brightness; // base_value -mm option (default 0.0)
		float contrast; // gain_value -mm option (default 1.0)
		Vector3d origin_offset; // -o u [v [w]] (default 0 0 0)
		Vector3d scale; // -s u [v [w]] (default 1 1 1)
		Vector3d turbulence; // -t u [v [w]] (default 0 0 0)
		char imfchan; // -imfchan (image file channel) r | g | b | m | l | z // bump default l, decal default m
	};

  class Texture : public Asset {
    friend class Debug;
    friend class WaveFrontObjMesh;

  public:
    Texture();
    virtual ~Texture() = 0;

    unsigned char* GetNativeTexturePtr();
    Color GetPixel(unsigned int x, unsigned int y);
    void SetPixel(unsigned int x, unsigned int y, const Color& color);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    unsigned int GetTextureID() const;

    Color GetColorKey() const;
    bool UseColorKey() const;

    void SetMipmapBias(float bias);
    void SetAnisoLevel(float level);
    void SetFilter(const FilterMode mode);
    void SetWrapMode(const WrapMode mode);
    void SetTextureFormat(const TextureFormat format);
    void SetTextureOption(const TextureOption& option);

    TextureDimension GetDimension() const;

  protected:
    unsigned char* nativeTexturePtr;
    unsigned int textureId;
    int width, height;
    float anisoLevel;
    float mipMapBias;

		TextureOption textureOption;
    FilterMode filterMode;
    WrapMode wrapMode;
    TextureDimension textureDimension;
    TextureFormat textureFormat;
    Color colorKey;
    bool useColorKey;
  };
}  // namespace Theodore

#endif /* Texture_h */
