#ifndef Node_h
#define Node_h

#include <list>
#include "Transform.h"

namespace Quark {
	class Node {
	public:
		Node();
		~Node();

		Node* GetParent() const;
		/*Transform GetWorldTransform() const;
		Transform GetLocalTransform() const;*/
		std::list<Node*> GetChildren() const;

		void AddChild(Node* child);
		virtual void UpdateQuadtree() = 0;

	private:
		Node* mParent;
		std::list<Node*> mChildren;
		Transform mWorldTransform;
		Transform mLocalTransform;
	};
}

#endif // Node_h