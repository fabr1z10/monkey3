#pragma once

#include "glm/glm.hpp"
#include <vector>
#include <monkey3/shapes/polygon.h>


//struct Polygon {
//	std::vector<glm::vec2> outer;
//
//	std::vector<std::vector<glm::vec2>> holes;
//};

struct Area {

	int id = -1;
	shapes::Polygon outer;
	std::vector<shapes::Polygon> holes;
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

	shapes::Polygon shape;

	bool active = true;
};