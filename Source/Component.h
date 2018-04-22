#ifndef Component_h
#define Component_h

#include "Object.h"
#include "Message.h"

namespace Quark {
	class Component : public Object {
		friend class GameObject;
	public:
		Component(const std::string& name) :Object(name) {}
		virtual ~Component() {}

	private:
		virtual MessageResult HandleMessage(Message& msg) {
			return MessageResult::Ignored;
		}

	protected:
		GameObject* mGameObject;
	};
}

#endif /* Component_h */