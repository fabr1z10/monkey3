#include <monkey3/components/walk2d.h>
#include <monkey3/utils.h>


Walk2D::Walk2D(const ControllerInfo& info) : Controller2D(info), _maxSpeed(info.maxSpeed),
	_accelerationTime(info.accelerationTime), _jumpHeight(info.jumpHeight), _timeToJumpApex(info.timeToJumpApex),
	_v(0.f), _a(0.f) {
	
	_gravity = (2.f * _jumpHeight) / (_timeToJumpApex * _timeToJumpApex);
	_jumpVelocity = (2.f * _jumpHeight) / _timeToJumpApex;
	_acceleration = _maxSpeed / _accelerationTime;

}

void Walk2D::start() {

}

void Walk2D::update(float dt) {

	control();

	//if (_flipH && _direction != 0) {
	//	m_node->setFlipX(_direction == -1);
	//}

	float maxSpeed{ _maxSpeed };

	if (grounded()) {
		_v.y = std::max(0.f, _v.y);
	}
	else {
		// bump head
		if (ceiling()) {
			_v.y = 0;
		}
	}

	// init acceleration with gravity
	_a = glm::vec3(0.f, -_gravity, 0.f);

	if (_direction == WalkDirection::RIGHT) {
		// x-acceleration should be always positive unless when !fliph && dir==-1		
		_a.x = _acceleration;		
	}
	else if (_direction == WalkDirection::LEFT) {
		_a.x = -_acceleration;
	}
	else {
		// apply deceleration only if velocity above threshold
		if (fabs(_v.x) > (_acceleration * dt) + 0.1f) {
			// decel, acceleration should be opposite velocity
			_a.x = -sign(_v.x) * _acceleration;
		}
		else {
			_a.x = 0.0f;
			_v.x = 0.0f;
		}
	}

	_v += _a * dt;
	// limit horizontal vel to max speed
	if (fabs(_v.x) > maxSpeed) {
		_v.x = sign(_v.x) * maxSpeed;
	}

	auto delta = _v * dt;
	move(delta);


}