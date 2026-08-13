#include <monkey3/shapes/polygon.h>
#include <monkey3/math/geometry.h>
#include <monkey3/math/geometry_detail.h>

using namespace shapes;

Polygon::Polygon(const std::vector<glm::vec2> &vertices) : Shape(), _points(vertices) {
	for (const auto& vertex : _points) {
		_bounds.addPoint(glm::vec3(vertex, 0.f));
	}
}

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

RayCastHit shapes::Polygon::raycastAxis(
    glm::vec3 origin,
    float length,
    Axis axis) const
{
    RayCastHit result;

    const int a = static_cast<int>(axis);
    const int other = 1 - a;

    // Ray is outside the polygon's bounding box on the
    // axis perpendicular to the ray.
    if (origin[other] < _bounds.min[other] ||
        origin[other] > _bounds.max[other])
    {
        return result;
    }

    // Ray starts inside the polygon.
    if (contains(glm::vec2(origin.x, origin.y))) {
        result.collide = true;
        result.length = 0.0f;
        return result;
    }

    glm::vec3 endPoint = origin;
    endPoint[a] += length;

    float closestDistance = std::abs(length);

    for (size_t i = 0; i < _points.size(); ++i) {
        const glm::vec2& p0 = _points[i];
        const glm::vec2& p1 = _points[(i + 1) % _points.size()];

        // Edge parallel to the ray.
        if (p0[other] == p1[other]) {
            continue;
        }

        // Find where the edge crosses the ray's fixed coordinate.
        const float t =
            (origin[other] - p0[other]) /
            (p1[other] - p0[other]);

        // Intersection is not on the edge.
        if (t < 0.0f || t > 1.0f) {
            continue;
        }

        // Coordinate of the intersection along the ray axis.
        const float intersection =
            p0[a] + t * (p1[a] - p0[a]);

        // Intersection must lie on the finite ray segment.
        if (length > 0.0f) {
            if (intersection < origin[a] ||
                intersection > endPoint[a])
            {
                continue;
            }
        }
        else {
            if (intersection > origin[a] ||
                intersection < endPoint[a])
            {
                continue;
            }
        }

        const float distance =
            std::abs(intersection - origin[a]);

        if (distance >= closestDistance) {
            continue;
        }

        closestDistance = distance;

        result.collide = true;
        result.length = distance;
        result.segmentIndex = static_cast<int>(i);

        // Points are CCW, therefore the right-hand
        // perpendicular is the outward normal.
        const glm::vec2 edge = p1 - p0;
        const glm::vec2 normal = glm::normalize(
            glm::vec2(edge.y, -edge.x));

        result.normal = glm::vec3(
            normal.x,
            normal.y,
            0.0f);
    }

    return result;
}