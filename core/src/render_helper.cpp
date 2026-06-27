#include <monkey3/render_helper.h>

void renderPoly(
		Renderer& r,
		const std::vector<glm::vec2>& points,
		bool closed,
		glm::vec4 color,
		glm::mat4 worldTransform)
{
	glm::vec2 prev = points[0];
	for (size_t i = 1; i < points.size(); i++) {
		auto next = points[i];
		r.submitLine(prev, next, color);
		prev = next;
	}
	if (closed) r.submitLine(points.back(), points.front(), color);
}