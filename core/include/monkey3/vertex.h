#pragma once

#include "glm/glm.hpp"
#include <GL/glew.h>
#include <cstddef> // for offsetof


struct QuadVertex {
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec4 color;
};

struct LineVertex {
	glm::vec3 pos;
	glm::vec4 color;
};

template<typename T>
struct VertexTraits;

template<>
struct VertexTraits<QuadVertex>
{
	static void setup()
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
							  sizeof(QuadVertex),
							  (void*)offsetof(QuadVertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
							  sizeof(QuadVertex),
							  (void*)offsetof(QuadVertex, uv));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
							  sizeof(QuadVertex),
							  (void*)offsetof(QuadVertex, color));
	}
};

template<>
struct VertexTraits<LineVertex>
{
	static void setup()
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
							  sizeof(LineVertex),
							  (void*)offsetof(LineVertex, pos));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
							  sizeof(LineVertex),
							  (void*)offsetof(LineVertex, color));
	}
};