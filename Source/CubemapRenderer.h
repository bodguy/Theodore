/**
	@brief rendering cubemap component
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef CubemapRenderer_h
#define CubemapRenderer_h

#include "MeshRenderer.h"

namespace Theodore {
	class CubemapRenderer : public MeshRenderer {
	public:
		CubemapRenderer();
		virtual ~CubemapRenderer();

		unsigned int GetTextureID() const;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

		unsigned int mTextureID;
	};
}

#endif /* CubemapRenderer_h */