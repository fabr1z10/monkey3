#include <monkey3/shapes/box.h>
#include <monkey3/primitive.h>

using namespace shapes;

Box::Box(float width, float height, glm::vec2 anchor) : Shape(), _size(width, height), _anchor(anchor) {
	glm::vec3 A{ -_anchor.x, -_anchor.y, 0.f };
	glm::vec3 B = A + glm::vec3(width, height, 0.f);
	_bounds = Bounds(A, B);

}

bool Box::contains(const glm::vec2& p) const {
	return (p.x >= -_anchor.x && p.x <= _anchor.x + _size.x &&
		p.y >= -_anchor.y && p.y <= _anchor.y + _size.y);
}

void Box::render(Renderer& r, glm::mat4 worldTransform) {
	glm::vec3 pos = glm::vec3(worldTransform[3]);
	glm::vec3 P0 = pos + glm::vec3(-_anchor, 0.f);
	glm::vec3 P1 = P0 + glm::vec3(_size.x, 0.f, 0.f);
	glm::vec3 P2 = P1 + glm::vec3(0, _size.y, 0.f);
	glm::vec3 P3 = P0 + glm::vec3(0, _size.y, 0.f);
	r.submitGeometry<LineInfo>(P0, P1, _color);
 	r.submitGeometry<LineInfo>(P1, P2, _color);
	r.submitGeometry<LineInfo>(P2, P3, _color);
	r.submitGeometry<LineInfo>(P3, P0, _color);

}

RayCastHit shapes::Box::raycastAxis(glm::vec3 origin, float length, Axis axis) const
{
	RayCastHit result;
	const int a = static_cast<int>(axis);
	const int other = 1 - a;

	if (origin[other] < _bounds.min[other] ||
		origin[other] > _bounds.max[other])
	{
		// this handle case of x-ray below or above the box
		// or y-ray left or right of the box
		return result;
	}

	if (contains(origin)) {
		// if ray starts from within the box the length should be 0
		// in this case normal doesn't make sense
		result.collide = true;
		result.length = 0;
		return result;
	}

	glm::vec3 endPoint = origin;
	endPoint[axis] += length;
	
	if (length > 0 && (origin[axis] < _bounds.min[axis] && endPoint[axis] >= _bounds.min[axis])) {
		result.collide = true;
		result.length = _bounds.min[axis] - origin[axis];
		result.normal[axis] = -1.f;
	}
	else if (length < 0 && (origin[axis] > _bounds.max[axis] && endPoint[axis] <= _bounds.max[axis])) {
		result.collide = true;
		result.length = origin[axis] - _bounds.max[axis];
		result.normal[axis] = 1.f;
	}
	return result;
}

