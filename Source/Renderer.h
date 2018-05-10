#ifndef Renderer_h
#define Renderer_h

#include "Component.h"
#include <vector>

namespace Quark {
	class Buffer; class VertexArray; class Program;
	class Renderer : public Component {
	public:
		Renderer(const std::string& name);
		virtual ~Renderer();

	protected:
		std::vector<Buffer*> mVbos;
		std::vector<Buffer*> mEbos;
		VertexArray* mVao;
		Program* mProgram;
	};
}

#endif /* Renderer_h */