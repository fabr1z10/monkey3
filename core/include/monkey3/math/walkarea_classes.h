#pragma once

#include "glm/glm.hpp"
#include <vector>



struct Polygon {
	std::vector<glm::vec2> outer;

	std::vector<std::vector<glm::vec2>> holes;
};

struct Area {
	int id = -1;
	Polygon shape;
	std::vector<int> portals;
};

struct Portal {
	int id = -1;

	int a_area = -1;
	int b_area = -1;

	glm::vec2 a;
	glm::vec2 b;

	bool enabled = true;
};

struct Obstacle {
	int id = -1;

	Polygon shape;

	bool active = true;
};