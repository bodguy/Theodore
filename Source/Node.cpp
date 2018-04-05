#include "Node.h"
#include "Transform.h"

namespace Quark {
	Node::Node() :mWorldTransform(new Transform()), mLocalTransform(new Transform()), mChildren(new std::list<Node*>()) {

	}

	Node::~Node() {

	}

	Node* Node::GetParent() const {
		return mParent;
	}

	Transform* Node::GetWorldTransform() const {
		return mWorldTransform;
	}

	Transform* Node::GetLocalTransform() const {
		return mLocalTransform;
	}
	
	std::list<Node*>* Node::GetChildren() const {
		return mChildren;
	}

	void Node::AddChild(Node* child) {
		child->mParent = this;
		mChildren->push_back(child);
	}

	void Node::Render() {
		for (Node* child : *mChildren) {
			child->Render();
		}
	}

	void Node::Update() {
		for (Node* child : *mChildren) {
			child->Update();
		}
	}
}