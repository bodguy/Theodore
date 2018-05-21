#ifndef Renderer_h
#define Renderer_h

#include "Component.h"
#include "Enumeration.h"
#include <vector>

namespace Quark {
	class Buffer; class VertexArray; class Program;
	class Renderer : public Component {
	public:
		Renderer(const std::string& name);
		virtual ~Renderer();

		void SetPrimitive(const Primitive primitive) { mPrimitive = primitive; }
		void SetDebugRender(bool debug) { mIsDebugRendering = debug; }

	protected:
		std::vector<Buffer*> mVbos;
		std::vector<Buffer*> mEbos;
		VertexArray* mVao;
		Program* mProgram;
		Primitive mPrimitive;
		bool mIsDebugRendering;
	};
}

#endif /* Renderer_h */