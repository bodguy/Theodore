/**
  @brief shadowinfo
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef ShadowInfo_h
#define ShadowInfo_h

namespace Theodore {
  class FrameBuffer;
  class Pipeline;
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