#include "monkey3/game.h"
#include "monkey3/xmlroomfactory.h"


int main() {
	Game g({320, 200}, {320, 200}, "Hello from Monkey!");
	auto factory = std::make_unique<XMLRoomFactory>(g);
	factory->setFilename("assets/rooms/44.yaml");
	g.setRoomFactory(std::move(factory));
	// need at least one pass, or render won't be called!
	//g.addRenderPass(RenderPass(320, 200, 1));
	g.init();
	g.run();

}

