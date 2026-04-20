#pragma once

#include "room.h"
#include <memory>

class Renderer;

class RoomFactory {
public:

	virtual std::unique_ptr<Room> createRoom(Renderer&) = 0;
};