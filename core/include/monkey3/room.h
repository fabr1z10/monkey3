#pragma once

#include "monkey3/renderer.h"
#include "monkey3/node.h"


class Room {
	// TODO!!!

public:
	virtual ~Room() = default;
	virtual void update(float dt) = 0;
	virtual void render(Renderer&, RenderContext) = 0;

private:
	std::unique_ptr<Node> _root;
};