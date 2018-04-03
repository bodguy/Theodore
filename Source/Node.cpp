#include "Node.h"
#include "Transform.h"

namespace Quark {
	Node::Node() :mWorldTransform(new Transform()), mLocalTransform(new Transform()), mChildren(new std::list<Node*>()) {

	}

	Node::~Node() {

	}

	Node* Node::getParent() const {
		return mParent;
	}

	Transform* Node::getWorldTransform() const {
		return mWorldTransform;
	}

	Transform* Node::getLocalTransform() const {
		return mLocalTransform;
	}
	
	std::list<Node*> Node::getChildren() const {
		return *mChildren;
	}

	void Node::addChild(Node* child) {
		child->mParent = this;
		mChildren->push_back(child);
	}

	void Node::update() {
		for (Node* child : *mChildren) {
			child->update();
		}
	}

	void Node::input() {
		for (Node* child : *mChildren) {
			child->input();
		}
	}

	void Node::render() {
		for (Node* child : *mChildren) {
			child->render();
		}
	}

	void Node::shutdown() {
		for (Node* child : *mChildren) {
			child->shutdown();
		}
	}
}