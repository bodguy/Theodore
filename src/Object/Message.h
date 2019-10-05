/**
  @brief gameobject message datastructure
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Message_h
#define Message_h

#include "../Graphics/Enumeration.h"

namespace Theodore {
  class Message {
    public:
    Message();
    explicit Message(MessageType type);
    Message(MessageType type, void* data);

    // copy, operator
    Message(const Message& rhs);
    Message& operator=(const Message& rhs);

    MessageType GetType() const;
    void* GetData() const;

    private:
    MessageType mType;
    void* mData;
  };
} // namespace Theodore
#endif /* Message_h */
