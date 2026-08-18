#include <monkey3/components/controller.h>
#include <monkey3/node.h>
#include <monkey3/room.h>
#include <monkey3/services/collisionengine.h>


Controller::Controller(const ControllerInfo& info) : Component(), _collisionStatus(0),
	_horizontalRayCount(info.horizontalRayCount), _verticalRayCount(info.verticalRayCount), _engine(nullptr) {
	setSize(info.size);

}

void Controller::start() {
	_engine = _node->getRoom()->getService<CollisionEngine>();

}

void Controller::setSize(glm::vec3 size) {
	_size = size;

	_halfWidth = 0.5f * size.x;
	
	_horizontalRaySpacing = size.y / (_horizontalRayCount - 1);
	
	_verticalRaySpacing = size.x / (_verticalRayCount - 1);
}

void Controller::resetCollisions() {
	_collisionStatus = 0;
}

void Controller::render(Renderer&, glm::mat4 worldTransform) {

}