#pragma once

#include "monkey3/component.h"
#include <glm/glm.hpp>

class Controller : public Component {
public:
	virtual void move(glm::vec3 delta) = 0;

private:
	glm::vec3 _size;
	// bit 0 = collide below, bit 1 = collide above, bit 2 = collide left, bit 3 = collide right
	uint8_t _collisionStatus;
};