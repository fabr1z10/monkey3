#pragma once


#include <monkey3/components/hotspot.h>


namespace components {
	class ObjectHotSpot : public HotSpot {
	public:
		ObjectHotSpot(std::unique_ptr<shapes::Shape> shape, int objectId);

		void onMouseEnter() override;

		void onMouseLeave() override;
	private:
		int _objectId;
	};
}