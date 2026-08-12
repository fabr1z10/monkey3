#pragma once

#include "monkey3/components/controller2d.h"

enum class WalkDirection {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Walk2D : public Controller2D {
public:
	void start() override;
	void update(double) override;
	virtual void control() = 0;
private:
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