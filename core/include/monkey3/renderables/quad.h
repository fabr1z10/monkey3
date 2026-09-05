#pragma once

#include "monkey3/renderable.h"

class Quad : public Renderable {

public:
	Quad(int texId, const Tex*, glm::ivec4 texCoords = glm::ivec4(), glm::vec2 anchor = glm::vec2(0.f));

	void render(Renderer&, glm::mat4 worldTransform) override;

private:
	int _texId;
	int _palId;
	glm::vec2 _anchor;
	glm::vec2 _size;
	glm::vec4 _coords;
};