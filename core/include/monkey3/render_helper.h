#pragma once

#include <glm/glm.hpp>
#include <monkey3/renderer.h>

void renderPoly(
		Renderer&,
		const std::vector<glm::vec2>& points,
		bool closed,
		glm::vec4 color,
		glm::mat4 worldTransform);