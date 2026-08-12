#include <monkey3/shapes/polygon.h>
#include <monkey3/math/geometry.h>
#include <monkey3/math/geometry_detail.h>

using namespace shapes;

Polygon::Polygon(const std::vector<glm::vec2> &vertices) : Shape(), _points(vertices) {}

bool Polygon::contains(const glm::vec2 &p) const {
	return pointInSimplePolygon(p, _points);
}

bool Polygon::segmentIntersects(const glm::vec2 &a, const glm::vec2 &b) const {
	return segmentIntersectsPolygonVec(a, b, _points);
}

void Polygon::render(Renderer & r, glm::mat4 worldTransform) {
	glm::vec3 pos = glm::vec3(worldTransform[3]);
	glm::vec3 start = pos + glm::vec3(_points[0], 0.f);
	glm::vec3 P0 = start;
	for (size_t i = 1; i < _points.size() ; i++) {
		glm::vec3 P1 = pos + glm::vec3(_points[i], 0.f);
		r.submitLine(P0, P1, {_color.r, _color.g, _color.b, _color.a});
		P0 = P1;
	}
	r.submitLine(P0, start, {_color.r, _color.g, _color.b, _color.a});
}