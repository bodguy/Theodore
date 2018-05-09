#ifndef SpriteRenderer_h
#define SpriteRenderer_h

#include "Component.h"
#include "Color.h"

namespace Quark {
	class Program; class Sprite;
	class SpriteRenderer : public Component {
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		void SetSprite(Sprite* sprite);
		void SetColor(const Color& color);
		void SetFlipX(const bool flipX);
		void SetFlipY(const bool flipY);

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Program* mProgram;
		Sprite* mSprite;
		Color mColor;
		bool mFlipX, mFlipY;
	};
}

#endif /* SpriteRenderer_h */