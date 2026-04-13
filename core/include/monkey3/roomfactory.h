#pragma once

#include "room.h"
#include <memory>

class RoomFactory {
public:

	virtual std::unique_ptr<Room> createRoom() = 0;
};