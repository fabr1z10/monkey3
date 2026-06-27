#include "monkey3/camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

Camera::Camera() {
	setPosition(glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::getProjectionMatrix() {
	return _projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	return _viewMatrix;
}

glm::mat4 Camera::getViewProjectionMatrix() {
	return getProjectionMatrix() * getViewMatrix();
}

OrthoCamera::OrthoCamera(float width, float height, float near, float far) : Camera(),
	_orthoSize(width, height) {
	float hw = _orthoSize.x / 2.0f;
	float hh = _orthoSize.y / 2.0f;
	_projectionMatrix = glm::ortho(-hw, hw, -hh, hh, -100.0f, 100.0f);
	//_halfSize = glm::vec3(hw, hh, 100.f);
}

void Camera::setPosition(glm::vec3 eye, glm::vec3 direction, glm::vec3 up) {
	_eye = clampPosition(eye);
	_fwd = glm::normalize(direction);
	_up = glm::normalize(up);
	updateViewMatrix();
}

void Camera::setPosition(glm::vec2 eye) {
	_eye = clampPosition({eye.x, eye.y, _eye.z});
	updateViewMatrix();
}

glm::vec3 Camera::clampPosition(const glm::vec3 &pos) {
	glm::vec3 result = pos;

	if (_xBounds) result.x = std::clamp(result.x, _xBounds->x, _xBounds->y);
	if (_yBounds) result.y = std::clamp(result.y, _yBounds->x, _yBounds->y);
	if (_zBounds) result.z = std::clamp(result.z, _zBounds->x, _zBounds->y);

	return result;
}

void Camera::updateViewMatrix() {
	_viewMatrix = glm::lookAt(_eye, _eye + _fwd, _up);
}

void Camera::setXBounds(float min, float max) {
	assert(min <= max);
	_xBounds = glm::vec2(min, max);
	_eye = clampPosition(_eye);
	updateViewMatrix();
}

void Camera::setYBounds(float min, float max) {
	assert(min <= max);
	_yBounds = glm::vec2(min, max);
	_eye = clampPosition(_eye);
	updateViewMatrix();
}

void Camera::setZBounds(float min, float max) {
	assert(min <= max);
	_zBounds = glm::vec2(min, max);
	_eye = clampPosition(_eye);
	updateViewMatrix();
}

void Camera::clearXBounds() { _xBounds.reset(); }
void Camera::clearYBounds() { _yBounds.reset(); }
void Camera::clearZBounds() { _zBounds.reset(); }

void Camera::clearBounds() {
	_xBounds.reset();
	_yBounds.reset();
	_zBounds.reset();
}