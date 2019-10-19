// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef FontRenderer_h
#define FontRenderer_h

#include "Renderer.h"

namespace Theodore {
    class Font;
    class FontRenderer : public Renderer {
    public:
        FontRenderer();
        FontRenderer(Font* font);
        ~FontRenderer();

        void SetFont(Font* font);

    private:
        virtual void Update(float deltaTime) override;
        virtual void Render() override;
        virtual bool CompareEquality(const Object& rhs) const override;
        virtual bool Destroy() override;

    private:
        Font* mFont;
    };
}

#endif // FontRenderer_h
