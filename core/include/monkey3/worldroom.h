#pragma once

#include "monkey3/room.h"
#include "monkey3/math/walkarea.h"
#include "monkey3/mouselistener.h"
#include "monkey3/services/hotspotmanager.h"

class WorldRoom : public Room {
public:
	WorldRoom(Game& game, glm::ivec2 size, glm::ivec4 gameView, glm::ivec4 uiView);

	void setCameraPosition(int x, int y);

private:
	glm::ivec2 _size;
	glm::ivec4 _gameView;
	glm::ivec4 _uiView;
};