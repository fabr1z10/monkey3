#pragma once

#include "monkey3/renderable.h"




class Lines : public Renderable {
public:
	Lines(std::vector<glm::vec2> points, glm::vec4 color, bool closed) : _points(points), _color(color) {};

	void render(Renderer&, glm::mat4 worldTransform) override;

	void setAnimation(int animId);
private:
	std::vector<glm::vec2> _points;
	glm::vec4 _color;
	bool _closed;
};