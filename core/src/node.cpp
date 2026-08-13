#include "monkey3/node.h"
#include "glm/gtc/matrix_transform.hpp"

Node::Node() : _position(0.f), _rotation(0.f), _scale(1.f, 1.f), _dirty(true) {
}

void Node::move(glm::vec3 delta) {
	_position += delta;
	updateLocalTransform();
}

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

glm::vec3 Node::getWorldPosition()
{
	if (_dirty) {
		updateWorldTransform();
	}
	return glm::vec3(_worldTransform[3]);
}


void Node::addChild(std::unique_ptr<Node> child) {
	child->_parent = this;
	child->markDirty();
	child->_room = _room;
	_children.push_back(std::move(child));
}

void Node::addRenderable(std::unique_ptr<Renderable> renderable) {
	_renderable = (std::move(renderable));
}

void Node::render(Renderer &r, const RenderContext& ctx) {

	bool visible = (_layerMask & ctx.layerMask) != 0;


	glm::mat4 worldTransform = getWorldTransform();
	if (visible && _renderable) {
		_renderable->render(r, worldTransform);
	}

	for (auto& c : _components) {
		c->render(r, worldTransform);
	}



		;// TO DO CALL RENDERABLE->REBDER _renderable->render(r, getWorldTransform());

	for (auto& child : _children)
		child->render(r, ctx);


}

void Node::addComponent(std::unique_ptr<Component> component) {
	component->setNode(this);
	_components.push_back(std::move(component));
}

void Node::update(float dt) {
	for (auto& component : _components) {
		component->update(dt);
	}

	for (auto& child : _children) {
		child->update(dt);
	}
}