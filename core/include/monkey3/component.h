#pragma once

#include "monkey3/renderable.h"

class Node;

class Component : public Renderable {
public:
	Component() : _active(true), _node(nullptr) {}

	virtual ~Component() = default;

	virtual void start() = 0;

	virtual void update(double) = 0;

	virtual void setNode(Node*);

	Node* getNode();

	bool isActive() const;

	void setActive(bool);

protected:
	Node* _node;
	bool _active;

};


inline Node* Component::getNode() {
	return _node;
}

inline void Component::setNode(Node* node) {
	_node = node;
}



inline bool Component::isActive() const {
	return _active;
}

inline void Component::setActive(bool value) {
	_active = value;
}
