#ifndef Component_h
#define Component_h

#include "Object.h"
#include "Message.h"

namespace Theodore {
	class GameObject; class Transform;
	class Component : public Object {
		friend class GameObject;
	public:
		Component(const std::string& name);
		virtual ~Component();

		Transform* GetTransform() const;
		bool IsEnabled() const;
		void SetEnabled(bool enable);

	private:
		virtual MessageResult HandleMessage(Message& msg);

	protected:
		GameObject* mGameObject;
		Transform* mTransform;
		bool mIsEnable;
	};
}

#endif /* Component_h */