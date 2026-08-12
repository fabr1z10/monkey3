#include <monkey3/components/hotspot.h>
#include <stdexcept>
#include <monkey3/adventureroom.h>

using namespace components;

HotSpot::~HotSpot() {
	auto* room = dynamic_cast<AdventureRoom*>(_node->getRoom());
	room->getHotSpotManager()->unregisterHotSpot(this);
}

void HotSpot::start() {

	auto* room =_node->getRoom()->getService<HotSpotManager>();
	room->registerHotSpot(this);
}


