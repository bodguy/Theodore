// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#ifndef Component_h
#define Component_h

#include "Object/Message.h"
#include "Object/Object.h"

namespace Theodore {
  class GameObject;
  class Transform;
  class Component : public Object {
    friend class GameObject;

  public:
    explicit Component(const std::string& name);
    virtual ~Component() override;

    Transform* GetTransform() const;
    bool IsEnabled() const;
    void SetEnabled(bool enable);

  private:
    virtual MessageResult HandleMessage(Message& msg);

  protected:
    GameObject* gameObject;
    Transform* transform;
    bool isEnable;
  };
}  // namespace Theodore

#endif /* Component_h */