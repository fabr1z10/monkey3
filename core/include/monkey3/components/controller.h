#pragma once

#include "monkey3/component.h"
#include <glm/glm.hpp>


class CollisionEngine;

struct ControllerInfo {
	glm::vec3 size;
	int maskUp;
	int maskDown;
	float skinWidth;
	int horizontalRayCount;
	int verticalRayCount;
	float maxClimbAngle;
	float maxDescendAngle;
	float maxSpeed;
	float accelerationTime;
	float jumpHeight;
	float timeToJumpApex;
};


class Controller : public Component {
public:
	Controller(const ControllerInfo&);

	void start() override;

	virtual void move(glm::vec3 delta) = 0;

	bool grounded() const;

	void setGrounded(bool);

	bool ceiling() const;

	void setCeiling();

	void render(Renderer&, glm::mat4 worldTransform) override;
	
protected:
	virtual void resetCollisions();

	void setSize(glm::vec3 _size);

	glm::vec3 _size;
	float _halfWidth;
	// bit 0 = collide below, bit 1 = collide above, bit 2 = collide left, bit 3 = collide right
	uint8_t _collisionStatus;
	
	int _horizontalRayCount;
	
	int _verticalRayCount;
	
	float _horizontalRaySpacing;
	
	float _verticalRaySpacing;

	CollisionEngine* _engine;

};

inline bool Controller::grounded() const {
	return (_collisionStatus & 0x01u) != 0;
}

inline bool Controller::ceiling() const {
	return (_collisionStatus & 0x02u) != 0;
}

inline void Controller::setGrounded(bool value) {
	if (value)
		_collisionStatus |= 0x01;
	else
		_collisionStatus &= 0xFE;
}

inline void Controller::setCeiling() {
	_collisionStatus |= 0x02u;
}