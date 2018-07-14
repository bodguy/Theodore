#ifndef Renderer_h
#define Renderer_h

#include "Component.h"
#include "Enumeration.h"
#include "Bounds.h"
#include <vector>

namespace Theodore {
	class Buffer; class VertexArray; class Pipeline;
	class Renderer : public Component {
	public:
		Renderer(const std::string& name);
		virtual ~Renderer();

		void SetPrimitive(const Primitive primitive);
		void SetVisibleGizmos(bool visible);

	protected:
		std::vector<Buffer*> mVbos;
		std::vector<Buffer*> mEbos;
		VertexArray* mVao;
		Pipeline* mProgram;
		Pipeline* mNormalVisualizeProgram;
		Primitive mPrimitive;
		// world space bounds
		Bounds mBounds;
		bool mIsVisibleGizmos;
	};
}

#endif /* Renderer_h */