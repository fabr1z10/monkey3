#include "monkey3/game.h"
#include <iostream>
#include <monkey3/shaders/lineshader.h>

class DoNothingRoom : public Room {
public:
	using Room::Room;

	void update(float) override {
		// do nothing
	}
	void render(const RenderContext& ctxx) override {
		glClearColor(1.f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_renderer.submitGeometry<LineInfo>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(100.f, 100.f, 0.f), glm::vec4(1.f));

	}
};

class TestRoomFactory : public RoomFactory {
public:
	using RoomFactory::RoomFactory;

	std::unique_ptr<Room> createRoom() override {
		return std::make_unique<DoNothingRoom>(_game);
	}
};


int main() {
	try {
		Game g({ 320, 200 }, { 320, 200 }, "Hello from Monkey!");
		g.setRoomFactory(std::make_unique<TestRoomFactory>(g));
		g.init();
		// need at least one pass, or render won't be called!
		g.addRenderPass(RenderPass(320, 200, 1));
		g.renderer().addShaderItem<LineRenderItem>(1000);
		g.run();
	}
	catch (const std::exception& e) {
		std::cerr << "\033[31mERROR: " << e.what() << "\033[0m\n";
		return 1;
	}


	
}