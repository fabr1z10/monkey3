#include "monkey3/node.h"
#include "glm/gtc/matrix_transform.hpp"

Node::Node() {}

void Node::setPosition(const glm::vec3& pos) {
	_position = pos;
	updateLocalTransform();
}

void Node::setPosition(float x, float y, float z) {
	Node::setPosition(glm::vec3(x, y, z));
}

void Node::setRotationDegrees(float degrees) {
	_rotation = glm::radians(degrees);
	updateLocalTransform();
}

void Node::setRotationRad(float radians) {
	_rotation = radians;
	updateLocalTransform();

}

void Node::setScale(float x, float y) {
	_scale = {x, y};
	updateLocalTransform();
}

void Node::updateLocalTransform() {
	glm::mat4 T = glm::translate(glm::mat4(1.0f), _position);

	glm::mat4 R = glm::rotate(
		glm::mat4(1.0f),
		_rotation,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glm::mat4 S = glm::scale(
		glm::mat4(1.0f),
		glm::vec3(_scale, 1.0f)
	);

	_localTransform = T * R * S;

	markDirty();

}

void Node::updateWorldTransform() {
	if (_parent) {
		_worldTransform = _parent->getWorldTransform() * _localTransform;
	} else {
		_worldTransform = _localTransform;
	}

	_dirty = false;

	for (auto& child : _children) {
		child->updateWorldTransform();
	}

}

void Node::markDirty() {
	if (_dirty) return;
	_dirty = true;
	for (auto& child : _children) {
		child->markDirty();
	}
}

glm::mat4 Node::getWorldTransform() {
	if (_dirty) {
		updateWorldTransform();
	}
	return _worldTransform;
}

void Node::addChild(std::unique_ptr<Node> child) {
	child->_parent = this;
	_children.push_back(std::move(child));
	child->markDirty();
}

void Node::render(Renderer &r, const RenderContext& ctx) {

	bool visible = (_layerMask & ctx.layerMask) != 0;

	if (visible && _renderable)
		;// TO DO CALL RENDERABLE->REBDER _renderable->render(r, getWorldTransform());

	for (auto& child : _children)
		child->render(r, ctx);


}
