#ifndef Node_h
#define Node_h

#include <list>

namespace Quark {
	class Transform;
	class Node {
	public:
		Node();
		~Node();

		Node* getParent() const;
		Transform* getWorldTransform() const;
		Transform* getLocalTransform() const;
		std::list<Node*> getChildren() const;

		void addChild(Node* child);
		void update();
		void input();
		void render();
		void shutdown();

	private:
		Node* mParent;
		std::list<Node*>* mChildren;
		Transform* mWorldTransform;
		Transform* mLocalTransform;
	};
}

#endif // Node_h