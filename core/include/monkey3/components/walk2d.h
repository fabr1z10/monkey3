#pragma once

#include "monkey3/components/controller2d.h"

enum class WalkDirection {
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Walk2D : public Controller2D {
public:
	Walk2D(const ControllerInfo&);
	void start() override;
	void update(float) override;
	virtual void control() = 0;
protected:
	float _gravity;
	float _jumpHeight;
	float _timeToJumpApex;
	float _jumpVelocity;
	float _acceleration;
	float _maxSpeed;
	float _accelerationTime;
	WalkDirection _direction;
	glm::vec3 _v;
	glm::vec3 _a;
};