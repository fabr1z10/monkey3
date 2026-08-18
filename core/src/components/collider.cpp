#include <monkey3/components/collider.h>
#include <monkey3/node.h>
#include <iostream>
#include <monkey3/room.h>
#include <monkey3/services/collisionengine.h>

using namespace shapes;

Collider::Collider(int flag, int mask, int tag) : _flag(flag), _mask(mask), _tag(tag), _engine(nullptr) {

}

Collider::~Collider() {
	if (_engine) {
		_engine->remove(this);
	}
}

void Collider::start() {
	_engine = _node->getRoom()->getService<CollisionEngine>();
	_engine->add(this);
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