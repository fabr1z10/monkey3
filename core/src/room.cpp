#include "monkey3/room.h"

Room::Room() {
     _root = std::make_unique<Node>();
}


void Room::render(Renderer & r, const RenderContext& ctx) {

     _root->render(r, ctx);
}
