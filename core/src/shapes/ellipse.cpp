#include <monkey3/shapes/ellipse.h>
#include <glm/gtc/constants.hpp> // Required for glm::two_pi
#include <algorithm>

using namespace shapes;

Ellipse::Ellipse(float rx, float ry) : Shape(), _radii(rx, ry) {
	_segments = std::clamp(
			static_cast<int>(std::ceil(std::max(rx, ry))),
			24,
			96);
	_bounds = Bounds(glm::vec3(-rx, -ry, 0.f), glm::vec3(rx, ry, 0.f));
}

bool Ellipse::contains(const glm::vec2 &p) const {
	// Degenerate cases
	if (_radii.x <= 0.0f || _radii.y <= 0.0f)
		return false;

	float x = p.x / _radii.x;
	float y = p.y / _radii.y;

	return x * x + y * y <= 1.0f;
}

void Ellipse::render(Renderer & r, glm::mat4 worldTransform) {
	float step = glm::two_pi<float>() / _segments;

	glm::vec3 pos = glm::vec3(worldTransform[3]);

	glm::vec3 prev = pos + glm::vec3(
		_radii.x * std::cos(0.f),
		_radii.y * std::sin(0.f),
		0);

	for (int i = 1; i <= _segments; ++i) {
		float t = i * step;

		glm::vec3 curr = pos + glm::vec3(
				_radii.x * std::cos(t),
				_radii.y * std::sin(t),
				0);

		r.submitLine(prev, curr, {_color.r, _color.g, _color.b, _color.a});

		prev = curr;
	}
}

RayCastHit Ellipse::raycastAxis(glm::vec3 origin, float length, Axis axis) const {
	// TODO
	return RayCastHit();
}
