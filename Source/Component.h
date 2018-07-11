#ifndef Component_h
#define Component_h

#include "Object.h"
#include "Message.h"

namespace Quark {
	class GameObject; class Transform;
	class Component : public Object {
		friend class GameObject;
	public:
		Component(const std::string& name);
		virtual ~Component();

		Transform* GetTransform() const;
		bool IsEnable() const;
		void SetEnable(bool enable);

	private:
		virtual MessageResult HandleMessage(Message& msg);

	protected:
		GameObject* mGameObject;
		Transform* mTransform;
		bool mIsEnable; // NEED FIX
	};
}

#endif /* Component_h */