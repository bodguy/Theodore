#ifndef KeyFrame_h
#define KeyFrame_h

namespace Theodore {
	class KeyFrame {
	public:
		KeyFrame(float time, float value);
		~KeyFrame();

	private:
		float mTime;
		float mValue;
	};
}

#endif /* KeyFrame_h */