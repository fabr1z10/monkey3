#include <monkey3/components/collider.h>
#include <monkey3/node.h>
#include <monkey3/room.h>
#include <monkey3/services/collisionengine.h>
#include <iostream>

using namespace shapes;

Collider::Collider(int flag, int mask, int tag) : _flag(flag), _mask(mask), _tag(tag) {

}

void Collider::start() {
	_engine = _node->getRoom()->getService<CollisionEngine>();

}

int Collider::getCollisionFlag() const {
	return _flag;
}

SimpleCollider::SimpleCollider(std::unique_ptr<Shape> shape, int flag, int mask, int tag) : Collider(flag, mask, tag), 
	_shape(std::move(shape)) {

}

Bounds Collider::getStaticBounds() {
	auto bounds = getShape()->getBounds();
	bounds.translate(_node->getWorldPosition());
	return bounds;

}


const Shape* SimpleCollider::getShape() const
{
	return _shape.get();
}

void SimpleCollider::render(Renderer& r, glm::mat4 worldTransform) {
	_shape->render(r, worldTransform);
}