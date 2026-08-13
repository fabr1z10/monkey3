#include "monkey3/game.h"
#include "monkey3/xmlroomfactory.h"
#include <iostream>

int main() {
	try {
		std::filesystem::path assets = PROJECT_ASSET_DIR;

		auto game = Game::load((assets / "supermario").string());
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
	catch (const std::exception& e) {
		std::cerr << "\033[31mERROR: " << e.what() << "\033[0m\n";
		return 1;
	}

}

