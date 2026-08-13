#include <monkey3/shapes/point.h>

using namespace shapes;

Point::Point(float x, float y) : Shape(), _coords(x, y) {
	_bounds = Bounds(glm::vec3(x, y, 0.f), glm::vec3(x, y, 0.f));
}

bool Point::contains(const glm::vec2& p) const {
	const float eps2 = 1e-8f;
	glm::vec2 d = p - _coords;
	return glm::dot(d, d) < eps2;
}

void Point::render(Renderer & r, glm::mat4 worldTransform) {
	glm::vec3 pos = glm::vec3(worldTransform[3]);
	glm::vec3 worldPos = pos + glm::vec3(_coords, 0.f);
	r.submitLine(worldPos, worldPos + glm::vec3(1, 1, 0), {_color.r, _color.g, _color.b, _color.a});
}

RayCastHit Point::raycastAxis(glm::vec3 origin, float length, Axis axis) const {
	return RayCastHit();
}

