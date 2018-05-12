#ifndef Sprite_h
#define Sprite_h

#include "Rect.h"
#include "Vector2d.h"
#include "Enumeration.h"
#include "VertexBuffer.h"

namespace Quark {
	class Texture2D; class Buffer; class VertexArray;
	class Sprite {
		friend class SpriteRenderer;
	public:
		Sprite();
		~Sprite();

		static Sprite* Create(Texture2D* texture, const Rect rect = Rect::zero);
		bool operator ==(const Sprite& rhs);
		bool operator !=(const Sprite& rhs);

	private:
		IndexFormat mFormat;
		Texture2D* mTexture;
		Rect mRect; // for a sprite with a single texture
		Rect mTextureRect; // for a sprite from an atlas
		Vector2d mTextureRectOffset;
		Vector2d mPivot;
		Vector2d mVertices[4];
		Vector2d mUvs[4];
		unsigned short mIndices[6];
	};
}

#endif /* Sprite_h */