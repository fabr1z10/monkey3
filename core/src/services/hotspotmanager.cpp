#include <monkey3/services/hotspotmanager.h>

HotSpotManager::HotSpotManager() : _hotspots() {
}

void HotSpotManager::registerHotSpot(components::HotSpot *hotspot) {

	_hotspots.insert(hotspot);
}

void HotSpotManager::unregisterHotSpot(components::HotSpot *hotspot) {

	_hotspots.erase(hotspot);
}