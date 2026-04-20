#pragma once

#include "monkey3/renderer.h"
#include "monkey3/node.h"


class Room {
	// TODO!!!

public:
	Room();
	virtual ~Room() = default;
	virtual void update(float dt) {}
	virtual void render(Renderer&, const RenderContext&);
	Node* getRoot();
private:
	std::unique_ptr<Node> _root;
};

inline Node *Room::getRoot() {

	return _root.get();
}
