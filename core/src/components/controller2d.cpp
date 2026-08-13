#include <monkey3/components/controller2d.h>
#include <monkey3/node.h>

Controller2D::Controller2D(const ControllerInfo& info) : Controller(info), _maxClimbAngle(info.maxClimbAngle),
_maxDescendAngle(info.maxDescendAngle) {
}


void Controller2D::updateRaycastOrigins() {
	auto pos = _node->getWorldPosition();
	_raycastOrigins.bottomLeft = pos.x - glm::vec3(_halfWidth, 0.f, 0.f);
	_raycastOrigins.bottomRight = pos.x + glm::vec3(_halfWidth, 0.f, 0.f);
	_raycastOrigins.topLeft = _raycastOrigins.bottomLeft + glm::vec3(0.f, _size.y, 0.f);
	_raycastOrigins.topRight = _raycastOrigins.bottomRight + glm::vec3(0.f, _size.y, 0.f);
}

void Controller2D::resetCollisions() {
	Controller::resetCollisions();
	_climbingSlope = false;
	_descendingSlope = false;
	_slopeAngleOld = _slopeAngle;
	_slopeAngle = 0.f;

}

void Controller2D::move(glm::vec3 delta) {

	if (delta == glm::vec3(0.f)) return;

	updateRaycastOrigins();

	float wasGrounded = grounded();

	resetCollisions();

	if (delta.y < 0 && wasGrounded) {
		descendSlope(delta);
	}

	_node->move(delta);
}


void Controller2D::descendSlope(glm::vec3 delta) {
	if (delta.x == 0.f) return;
	auto r0 = delta.x > 0.f ? _raycastOrigins.bottomLeft : _raycastOrigins.bottomRight;


}