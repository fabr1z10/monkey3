#pragma once

#include "room.h"
#include <memory>

class Renderer;
class Game;


class RoomFactory {
public:
	RoomFactory(Game& game) : _game(game) {}

	virtual std::unique_ptr<Room> createRoom() = 0;

protected:
	Game& _game;
};