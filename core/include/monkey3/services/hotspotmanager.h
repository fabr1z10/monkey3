#pragma once

#include <monkey3/components/hotspot.h>
#include <unordered_set>
#include <monkey3/service.h>

class HotSpotManager : public IService {
public:

	HotSpotManager();

	void registerHotSpot(components::HotSpot* hotspot);

	void unregisterHotSpot(components::HotSpot* hotspot);


private:
	std::unordered_set<components::HotSpot*> _hotspots;
};
