#pragma once

#include "glm/glm.hpp"
#include "renderer.h"
#include <memory>

class Node {
public:

	virtual void render(Renderer& r) {}
private:
	uint32_t layerMask = 1;			// default = World

	glm::mat4 localTransform;

	glm::mat4 worldTransform;

	std::vector<std::unique_ptr<Node>> _children;

};