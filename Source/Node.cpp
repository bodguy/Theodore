#include "Node.h"

namespace Quark {
	Node::Node() :mParent(NULL), mChildren(), mWorldTransform(), mLocalTransform() {

	}

	Node::~Node() {

	}

	Node* Node::GetParent() const {
		return mParent;
	}

	Transform Node::GetWorldTransform() const {
		return mWorldTransform;
	}

	Transform Node::GetLocalTransform() const {
		return mLocalTransform;
	}
	
	std::list<Node*> Node::GetChildren() const {
		return mChildren;
	}

	void Node::AddChild(Node* child) {
		child->mParent = this;
		mChildren.push_back(child);
	}
}