#ifndef Node_h
#define Node_h

#include <list>

namespace Quark {
	class Transform;
	class Node {
	public:
		Node();
		~Node();

		Node* GetParent() const;
		Transform* GetWorldTransform() const;
		Transform* GetLocalTransform() const;
		std::list<Node*> GetChildren() const;

		void AddChild(Node* child);
		virtual void Update();
		virtual void Input();
		virtual void Render();
		virtual void Shutdown();

	private:
		Node* mParent;
		std::list<Node*>* mChildren;
		Transform* mWorldTransform;
		Transform* mLocalTransform;
	};
}

#endif // Node_h