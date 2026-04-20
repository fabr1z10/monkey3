#pragma once

#include "glm/glm.hpp"
#include "monkey3/renderer.h"
#include "monkey3/renderable.h"
#include <memory>

class Node {
public:
	Node();

	void setLayerMask(uint32_t);

	uint32_t getLayerMask() const;

	glm::mat4 getWorldTransform();

	void render(Renderer& r, const RenderContext&);

	void setPosition(const glm::vec3& pos);

	void setPosition(float x, float y, float z);

	void setRotationDegrees(float degrees);

	void setRotationRad(float radians);

	void setScale(float x, float y);

	void addChild(std::unique_ptr<Node> child);

	void addRenderable(std::unique_ptr<Renderable> renderable);
protected:
	void updateLocalTransform();
	void updateWorldTransform();
	void markDirty();

private:
	uint32_t _layerMask = 1;			// default = World

	glm::mat4 _localTransform{1.f};

	glm::mat4 _worldTransform{1.f};

	std::vector<std::unique_ptr<Node>> _children;

	Node* _parent = nullptr;

	glm::vec3 _position{0.f}; // z = layer

	float _rotation = 0.f;			// around Z axis only

	glm::vec2 _scale{1.f, 1.f};

	bool _dirty = true;		// true if world matrix needs to be recomputed

	std::unique_ptr<Renderable> _renderable;
};

inline void Node::setLayerMask(uint32_t layerMask) {
	_layerMask = layerMask;
}

inline uint32_t Node::getLayerMask() const {
	return _layerMask;
}




