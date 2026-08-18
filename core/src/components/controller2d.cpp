#include <monkey3/components/controller2d.h>
#include <monkey3/services/collisionengine.h>
#include <monkey3/node.h>
#include <monkey3/utils.h>

Controller2D::Controller2D(const ControllerInfo& info) : Controller(info), _maxClimbAngle(info.maxClimbAngle),
_maxDescendAngle(info.maxDescendAngle), _maskUp(info.maskUp), _maskDown(info.maskDown), _skinWidth(info.skinWidth) {
}


void Controller2D::updateRaycastOrigins() {
	auto pos = _node->getWorldPosition();
	_raycastOrigins.bottomLeft = pos - glm::vec3(_halfWidth, 0.f, 0.f);
	_raycastOrigins.bottomRight = pos + glm::vec3(_halfWidth, 0.f, 0.f);
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
	if (!isEqual(delta.y, 0.f)) {
		verticalCollisions(delta);
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


void Controller2D::verticalCollisions(glm::vec3& velocity) {
	auto directionY = sign(velocity.y);
	auto directionX = sign(velocity.x);
	bool goingForward = velocity.x > 0.0f;
	//bool faceRight = !m_node->getFlipX();
	float rayLength = std::abs(velocity.y) + _skinWidth;
	float obstacleDistance = std::numeric_limits<float>::max();
	//std::unordered_set<Node*> obstacles;
	Node* obstacle = nullptr;
	float speedX = fabs(velocity.x);
	bool atleast = false;
	//glm::vec3 r0(m_raycastOrigins.xMin, 0.f, 0.f);
	glm::vec3 r0 = directionY > 0.f ? _raycastOrigins.topLeft : _raycastOrigins.bottomLeft;
	for (int i = 0; i < _verticalRayCount; i++) {
		auto rayOrigin = r0 + glm::vec3(i * _verticalRaySpacing + velocity.x, 0.f, 0.f) ;
		int collMask = (directionY == -1 ? _maskDown : _maskUp);
		RayCastHit hit = _engine->rayCastAxis(rayOrigin, directionY * rayLength, collMask, _node, Axis::Y);
		bool ciao = grounded();
		if (hit.collide) {
			//if (!m_wasGnd) {
			//}
			//std::cout << "raylen = " << rayLength << "; dist = " << hit.length << "\n";
			atleast = true;
			velocity.y = (hit.length - _skinWidth) * directionY;
			rayLength = hit.length;
			if (_climbingSlope) {
				velocity.x = (velocity.y / tan(_slopeAngle)) * directionX;
			}
			if (directionY == -1) {
				setGrounded(true);
			} else {
				setCeiling();
			}
//			m_details.below = directionY == -1;
//			m_details.above = directionY == 1;
			//if (directionY < 0.0f) {
			if (hit.length < obstacleDistance) {
				//obstacle = hit.entity->getNode();
				obstacleDistance = hit.length;
			}
			//obstacles.insert(hit.entity->getNode());
			//}
		}
	}


	// register to new platforms -- needed for moving platforms+
//	if (obstacle != nullptr) {
//		auto* platformController = obstacle->getComponent<Platform>();
//
//		if (directionY < 0.f) {
//			if (platformController != m_platforms) {
//				if (platformController != nullptr) {
//					platformController->registerComponent(this);
//				}
//				if (m_platforms != nullptr) {
//					m_platforms->unregisterComponent(this);
//				}
//				m_platforms = platformController;
//			}
//		} else {
//			// notify hit
//			if (platformController != nullptr) platformController->hitFromBelow();
//		}
//	} else {
//		if (m_platforms != nullptr && !forced) {
//			m_platforms->unregisterComponent(this);
//			m_platforms = nullptr;
//		}
//	}

}