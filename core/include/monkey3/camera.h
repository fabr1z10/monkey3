#pragma once

#include "glm/glm.hpp"
#include <optional>

class Camera {
public:
	Camera();

	glm::mat4 getProjectionMatrix();

	glm::mat4 getViewMatrix();

	glm::mat4 getViewProjectionMatrix();

	void setPosition(glm::vec3 eye, glm::vec3 dir, glm::vec3 up);

protected:
	glm::vec3 clampPosition(const glm::vec3& pos);
	void updateViewMatrix();
	// each camera has a viewport i.e. the rectangle of the screen that this camera renders into.
	// viewport 0 means --> camera renders to the whole screen
	glm::vec3 _eye;
	glm::vec3 _fwd;
	glm::vec3 _up;

	glm::mat4 _projectionMatrix;

	glm::mat4 _viewMatrix;

	std::optional<glm::vec2> _xBounds;
	std::optional<glm::vec2> _yBounds;
	std::optional<glm::vec2> _zBounds;
};

class OrthoCamera : public Camera {
public:
	OrthoCamera(float width, float height, float near, float far);

	glm::vec2 getSize() const;

private:
	glm::vec2 _orthoSize;
};