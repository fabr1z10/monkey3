#include <monkey3/math/geometry_detail.h>
#include <monkey3/math/geometry.h>

bool segmentIntersectsPolygonVec(
		const glm::vec2& a,
		const glm::vec2& b,
		const std::vector<glm::vec2>& poly)
{
	int n = (int)poly.size();

	for (int i = 0; i < n; i++)
	{
		const glm::vec2& p1 = poly[i];
		const glm::vec2& p2 = poly[(i + 1) % n];

		if (segmentsIntersect(a, b, p1, p2))
			return true;
	}

	return false;
}