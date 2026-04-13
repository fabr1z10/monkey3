#pragma once

#include "glm/glm.hpp"

struct QuadVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec4 color;
};

struct LineVertex {
	glm::vec3 pos;
	glm::vec4 color;
};