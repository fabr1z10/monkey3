#pragma once

#include <monkey3/components/walk2d.h>
#include <monkey3/keylistener.h>

class Game;

class PlayerWalk2D : public Walk2D {
public:
	PlayerWalk2D(Game&, const ControllerInfo&);

	void control() override;
private:
	Game& _game;
};