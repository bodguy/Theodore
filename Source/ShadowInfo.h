#ifndef ShadowInfo_h
#define ShadowInfo_h

namespace Quark {
	class FrameBuffer; class Pipeline;
	class ShadowInfo {
	public:
		ShadowInfo();
		~ShadowInfo();

	private:
		FrameBuffer* frame;
		Pipeline* shadowProgram;
	};
}

#endif /* ShadowInfo_h */