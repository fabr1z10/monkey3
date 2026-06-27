#include <monkey3/math/geometry.h>
#include <monkey3/math/geometry_detail.h>

float orient(const glm::vec2& a,
					const glm::vec2& b,
					const glm::vec2& c)
{
	return (b.x - a.x) * (c.y - a.y)
		   - (b.y - a.y) * (c.x - a.x);
}

bool pointOnSegment(const glm::vec2& a,
						   const glm::vec2& b,
						   const glm::vec2& p)
{
	float crossp = (b.x - a.x) * (p.y - a.y)
				   - (b.y - a.y) * (p.x - a.x);

	if (fabs(crossp) > 1e-5f)
		return false;

	float dotp = (p.x - a.x) * (p.x - b.x)
				 + (p.y - a.y) * (p.y - b.y);

	return dotp <= 0;
}

bool pointInSimplePolygon(const glm::vec2& p,
	const std::vector<glm::vec2>& poly)
{
	if (poly.empty()) return false;

	if (poly.size() == 2) {
		return pointOnSegment(poly[0], poly[1], p);
	}

	if (poly.size() == 1) {
		return poly[0] == p;
	}

	// 🔥 STEP 0: boundary check (IMPORTANT)
	for (int i = 0; i < poly.size(); i++)
	{
		const auto& a = poly[i];
		const auto& b = poly[(i + 1) % poly.size()];

		if (pointOnSegment(p, a, b))
			return true; // treat boundary as INSIDE
	}

	bool inside = false;
	int n = (int)poly.size();

	for (int i = 0, j = n - 1; i < n; j = i++) {

		const auto& a = poly[i];
		const auto& b = poly[j];

		bool intersect =
				((a.y > p.y) != (b.y > p.y)) &&
				(p.x < (b.x - a.x) * (p.y - a.y) /
					   (b.y - a.y + 1e-12f) + a.x);

		if (intersect)
			inside = !inside;
	}

	return inside;
}

bool pointInPolygon(const glm::vec2& p,
					const Polygon& poly)
{
	// 1. Must be inside outer boundary
	if (!pointInSimplePolygon(p, poly.outer))
		return false;

	// 2. Must NOT be inside any hole
	for (const auto& hole : poly.holes)
	{
		if (pointInSimplePolygon(p, hole))
			return false;
	}

	return true;
}



bool segmentIntersectsPolygon(
		glm::vec2 a,
		glm::vec2 b,
		const Polygon& poly)
{
	// 1. check outer boundary
	if (segmentIntersectsPolygonVec(a, b, poly.outer))
		return true;

	// 2. check holes (IMPORTANT)
	for (const auto& hole : poly.holes)
	{
		if (segmentIntersectsPolygonVec(a, b, hole))
			return true;
	}

	return false;
}

bool segmentsIntersect(const glm::vec2& p1,
							  const glm::vec2& p2,
							  const glm::vec2& q1,
							  const glm::vec2& q2)
{
	float o1 = orient(p1, p2, q1);
	float o2 = orient(p1, p2, q2);
	float o3 = orient(q1, q2, p1);
	float o4 = orient(q1, q2, p2);

	// general case
	if ((o1 > 0) != (o2 > 0) &&
		(o3 > 0) != (o4 > 0))
		return true;

	// collinear cases
	if (fabs(o1) < 1e-6f && isInAABB(p1, p2, q1)) return true;
	if (fabs(o2) < 1e-6f && isInAABB(p1, p2, q2)) return true;
	if (fabs(o3) < 1e-6f && isInAABB(q1, q2, p1)) return true;
	if (fabs(o4) < 1e-6f && isInAABB(q1, q2, p2)) return true;

	return false;
}

bool isInAABB(const glm::vec2& a,
			  const glm::vec2& b,
			  const glm::vec2& p)
{
	return (p.x >= std::min(a.x, b.x) &&
			p.x <= std::max(a.x, b.x) &&
			p.y >= std::min(a.y, b.y) &&
			p.y <= std::max(a.y, b.y));
}

bool segmentIntersectsLoop(const glm::vec2& A,
						   const glm::vec2& B,
						   const std::vector<glm::vec2>& loop)
{
	if (loop.size() < 2) return false;

	for (size_t i = 0; i < loop.size(); i++)
	{
		const glm::vec2& C = loop[i];
		const glm::vec2& D = loop[(i + 1) % loop.size()];

		if (segmentsIntersect(A, B, C, D))
			return true;
	}

	return false;
}

