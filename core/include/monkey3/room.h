#pragma once

#include "monkey3/renderer.h"

class Room {
	// TODO!!!

public:
	virtual ~Room() = default;
	virtual void update(float dt) = 0;
	virtual void render(Renderer&) = 0;

};