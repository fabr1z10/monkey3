#include <monkey3/components/controller2d.h>
#include <monkey3/services/collisionengine.h>
#include <monkey3/node.h>
#include <monkey3/utils.h>

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

	if (!isEqual(delta.x, 0.f)) {
		horizontalCollisions(delta);
	}
	_node->move(delta);
}


void Controller2D::climbSlope(glm::vec3& velocity, float slopeAngle) {
	float moveDistance = fabs(velocity.x);
	float climbVelocityY = sin(slopeAngle) * moveDistance;
	if (velocity.y <= climbVelocityY) {
		velocity.y = climbVelocityY;
		velocity.x = cos(slopeAngle) * moveDistance * sign(velocity.x);
		setGrounded(true);
		//m_details.below = true;
		_climbingSlope = true;
		_slopeAngle = slopeAngle;
	}
}

void Controller2D::descendSlope(glm::vec3& delta) {
	if (delta.x == 0.f) return;
	auto r0 = delta.x > 0.f ? _raycastOrigins.bottomLeft : _raycastOrigins.bottomRight;
	auto hit = _engine->rayCastAxis(r0, 100.f, _maskDown, _node, Axis::Y);
	if (hit.collide) {
		float slopeAngle = angle(hit.normal, glm::vec3(0.f, 1.f, 0.f));
		if (slopeAngle != 0.f && slopeAngle <= _maxDescendAngle && (sign(hit.normal.x) == sign(delta.x))) {
			if (hit.length - _skinWidth <= std::tan(glm::radians(slopeAngle)) * std::abs(delta.x)) {
				float moveDistance = std::abs(delta.x);
				float descendDeltaY = std::sin(slopeAngle) * moveDistance;
				delta.y -= descendDeltaY;
				delta.x = std::cos(slopeAngle) * moveDistance * (delta.x > 0.f ? 1.f : -1.f);
				_slopeAngle = slopeAngle;
				setGrounded(true);
				_descendingSlope = true;
			}
		}
	}
}

void Controller2D::horizontalCollisions(glm::vec3& delta) {
	float directionX = sign(delta.x);
	float rayLength = std::abs(delta.x) + _skinWidth;
	auto r0 = directionX == -1.f ? _raycastOrigins.bottomLeft : _raycastOrigins.bottomRight;
	for (int i = 0; i < _horizontalRayCount; ++i) {
		auto r = r0 + glm::vec3(0.f, i * _horizontalRaySpacing, 0.f);
		auto hit = _engine->rayCastAxis(r, rayLength * directionX, _maskDown, _node, Axis::X);
		if (hit.collide) {
			float slopeAngle = angle(hit.normal, glm::vec3(0.f, 1.f, 0.f));
			if (i == 0 && slopeAngle <= _maxClimbAngle) {
				if (_descendingSlope) {
					_descendingSlope = false;
					delta.y = 0.f;
				}
				float distanceToSlopeStart = 0.f;//
				if (_slopeAngle != _slopeAngleOld) {
					distanceToSlopeStart = hit.length - _skinWidth;
					delta.x -= distanceToSlopeStart * directionX;
				}
				climbSlope(delta, slopeAngle);
				delta.x += distanceToSlopeStart * directionX;
			}
			if (!_climbingSlope || slopeAngle > _maxClimbAngle) {
				delta.x = (hit.length - _skinWidth) * directionX;
				rayLength = hit.length;
				if (_climbingSlope) {
					delta.y = std::tan(_slopeAngle) * std::abs(delta.x);
				}
				setGrounded(true);
			}
		}
	}
}