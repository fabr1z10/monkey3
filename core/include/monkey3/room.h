#pragma once

#include "monkey3/renderer.h"
#include "monkey3/node.h"


class Game;


class Room {
	// TODO!!!

public:
	Room(Game&);
	virtual ~Room() = default;
	virtual void update(float dt) {}
	virtual void render(const RenderContext&);
	Node* getRoot();
private:
	std::unique_ptr<Node> _root;
protected:
	Game& _game;
	Renderer& _renderer;
};

inline Node *Room::getRoot() {

	return _root.get();
}
