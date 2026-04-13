#pragma once

#include "glm/glm.hpp"
#include "vertex.h"
#include "batch.h"
#include "shader.h"
#include <optional>


class Renderer {

public:

	void init();

	void initBatches();

	void submitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& uvRect, const glm::vec4& color);

	void submitLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

private:
	Batch<QuadVertex> _quadBatch;
	Batch<LineVertex> _lineBatch;
	std::optional<Shader> _quadShader;
	std::optional<Shader> _lineShader;

};