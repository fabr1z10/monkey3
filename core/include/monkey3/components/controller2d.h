#pragma once

#include "monkey3/components/controller.h"


struct RaycastOrigins {
	glm::vec3 topRight;
	glm::vec3 topLeft;
	glm::vec3 bottomRight;
	glm::vec3 bottomLeft;
};

class Controller2D: public Controller {
public:
	Controller2D(const ControllerInfo&);

	void move(glm::vec3 delta) override;
protected:
	void resetCollisions() override;

private:
	// the following three methods can modify delta, that's why delta is passed by reference
	void climbSlope(glm::vec3& velocity, float slopeAngle);

	void descendSlope(glm::vec3& delta);

	void horizontalCollisions(glm::vec3& delta);

	void verticalCollisions(glm::vec3& delta);

	void updateRaycastOrigins();

	float _maxClimbAngle;
	float _maxDescendAngle;
	RaycastOrigins _raycastOrigins;
	bool _climbingSlope;
	bool _descendingSlope;
	float _slopeAngle;
	float _slopeAngleOld;
	int _maskUp;
	int _maskDown;
	float _skinWidth;

};