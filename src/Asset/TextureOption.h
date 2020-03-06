// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef TextureOption_h
#define TextureOption_h

#include "Math/Vector3d.h"

namespace Theodore {
	class TextureOption {
	public:
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
}

#endif /* TextureOption_h */
