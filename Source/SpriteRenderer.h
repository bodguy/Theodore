#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "Renderer.h"
#include "Color.h"

namespace Theodore {
	class Sprite;
	class SpriteRenderer : public Renderer {
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		void SetSprite(Sprite* sprite);
		void SetColor(const Color& color);
		void SetFlipX(const bool flipX);
		void SetFlipY(const bool flipY);
		Color GetColor() const;
		bool GetFlipX() const;
		bool GetFlipY() const;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Sprite* mSprite;
		Color mColor;
		bool mFlipX, mFlipY;
	};
}

#endif /* SpriteRenderer_h */