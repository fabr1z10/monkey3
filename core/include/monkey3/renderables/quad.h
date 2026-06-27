#pragma once

#include "monkey3/renderable.h"

class Quad : public Renderable {

public:
	Quad(Renderer&, const std::string& texture, glm::vec2 anchor = glm::vec2(0.f));

	void render(Renderer&, glm::mat4 worldTransform) override;

private:
	int _texId;
	glm::vec2 _anchor;
	glm::vec2 _size;
};