/**
  @brief keyframe animation datastructure
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

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
} // namespace Theodore

#endif /* KeyFrame_h */