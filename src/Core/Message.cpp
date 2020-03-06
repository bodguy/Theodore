// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Message.h"

namespace Theodore {
  Message::Message() : messageType(), messagePayload(nullptr) {}

  Message::Message(MessageType type) : messageType(type), messagePayload(nullptr) {}

  Message::Message(MessageType type, void* payload) : messageType(type), messagePayload(payload) {}

  Message::Message(const Message& rhs) {
		messageType = rhs.messageType;
		messagePayload = rhs.messagePayload;
  }

  Message& Message::operator=(const Message& rhs) {
		messageType = rhs.messageType;
		messagePayload = rhs.messagePayload;
    return *this;
  }

  MessageType Message::GetType() const { return messageType; }

  void* Message::GetData() const { return messagePayload; }
}  // namespace Theodore
