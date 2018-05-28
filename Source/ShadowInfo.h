#ifndef ShadowInfo_h
#define ShadowInfo_h

namespace Quark {
	class FrameBuffer; class Program;
	class ShadowInfo {
	public:
		ShadowInfo();
		~ShadowInfo();

	private:
		FrameBuffer* frame;
		Program* shadowProgram;
	};
}

#endif /* ShadowInfo_h */