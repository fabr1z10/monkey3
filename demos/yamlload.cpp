#include "monkey3/game.h"
#include "monkey3/xmlroomfactory.h"


int main() {

	auto game = Game::load("/home/fabrizio/monkey3/data/supermario");
	//Game g({320, 200}, {320, 200}, "Hello from Monkey!");
	//g.assetManager().setAssetFile("/home/fabrizio/monkey3/data/supermario/mario.yaml");
	auto factory = std::make_unique<XMLRoomFactory>(*game.get());
	factory->setFilename("00.yaml");
	game->setRoomFactory(std::move(factory));
	// need at least one pass, or render won't be called!
	//g.addRenderPass(RenderPass(320, 200, 1));
	game->init();
	game->run();

}

