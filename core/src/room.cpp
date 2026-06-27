#include "monkey3/room.h"
#include "monkey3/game.h"

Room::Room(Game& game) : _game(game), _renderer(game.renderer()) {
     _root = std::make_unique<Node>();
}


void Room::render(const RenderContext& ctx) {

     _root->render(_renderer, ctx);
}

