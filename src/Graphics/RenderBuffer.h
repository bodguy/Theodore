/**
  @brief OpenGL renderbuffer class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef RenderBuffer_h
#define RenderBuffer_h

namespace Theodore {
  class RenderBuffer {
    public:
    RenderBuffer();
    ~RenderBuffer();

    unsigned int GetRenderBufferID() const;
    void Storage(unsigned int width, unsigned int height);

    private:
    unsigned int mRenderBufferID;
  };
} // namespace Theodore

#endif /* RenderBuffer_h */
