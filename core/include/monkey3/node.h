#pragma once

#include "glm/glm.hpp"
#include "monkey3/renderer.h"
#include "monkey3/renderable.h"
#include "component.h"
#include <memory>


class Room;


class Node {
public:
	Node();

	void setLayerMask(uint32_t);

	uint32_t getLayerMask() const;

	glm::mat4 getWorldTransform();

	glm::vec3 getWorldPosition();

	void render(Renderer& r, const RenderContext&);

	void setPosition(const glm::vec3& pos);

	void setPosition(float x, float y, float z);

	void move(glm::vec3 delta);

	void setRotationDegrees(float degrees);

	void setRotationRad(float radians);

	void setScale(float x, float y);

	void addChild(std::unique_ptr<Node> child);

	void addComponent(std::unique_ptr<Component> component);

	void addRenderable(std::unique_ptr<Renderable> renderable);

	Room* getRoom() const;

	void setRoom(Room* room);

	void update(float dt);
protected:
	void updateLocalTransform();
	void updateWorldTransform();
	void markDirty();

private:
	uint32_t _layerMask = 1;			// default = World

	glm::mat4 _localTransform{1.f};

	glm::mat4 _worldTransform{1.f};

	std::vector<std::unique_ptr<Node>> _children;

	std::vector<std::unique_ptr<Component>> _components;

	Node* _parent = nullptr;

	glm::vec3 _position{0.f}; // z = layer

	float _rotation = 0.f;			// around Z axis only

	glm::vec2 _scale{1.f, 1.f};

	bool _dirty = true;		// true if world matrix needs to be recomputed

	std::unique_ptr<Renderable> _renderable;

	// the room node belongs to (if any)
	Room* _room = nullptr;

};

inline void Node::setLayerMask(uint32_t layerMask) {
	_layerMask = layerMask;
}

inline uint32_t Node::getLayerMask() const {
	return _layerMask;
}

inline Room *Node::getRoom() const {

	return _room;
}

inline void Node::setRoom(Room *room) {
	_room = room;
}