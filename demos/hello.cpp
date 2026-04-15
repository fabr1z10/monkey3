#include "monkey3/game.h"

class DoNothingRoom : public Room {
public:
	void update(float) override {
		// do nothing
	}
	void render(Renderer&, RenderContext) override {
		glClearColor(1.f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
};

class TestRoomFactory : public RoomFactory {
public:
	std::unique_ptr<Room> createRoom() override {
		return std::make_unique<DoNothingRoom>();
	}
};


int main() {
	Game g({320, 200}, {320, 200}, "Hello from Monkey!");
	g.setRoomFactory(std::make_unique<TestRoomFactory>());
	// need at least one pass, or render won't be called!
	g.addRenderPass(RenderPass(320, 200, 1));
	g.init();
	g.run();
	
}