#pragma once

#include "monkey3/renderer.h"
#include "monkey3/node.h"


class Room {
	// TODO!!!

public:
	Room();
	virtual ~Room() = default;
	virtual void update(float dt) = 0;
	virtual void render(Renderer&, const RenderContext&);

private:
	std::unique_ptr<Node> _root;
};