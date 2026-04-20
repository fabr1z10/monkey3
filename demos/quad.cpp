#include "monkey3/game.h"

class TestRoomFactory : public RoomFactory {
public:
    std::unique_ptr<Room> createRoom(Renderer& renderer) override {
        auto room = std::make_unique<Room>();
        renderer.registerTexture("assets/images/kq1.png");

        auto node = std::make_unique<Node>();
        node->addRenderable(std::make_unique<QuadRenderable>(0, glm::vec4(30/1024.,1/1024.,28/1024.,40/1024.), glm::vec2(28,40)));
        room->getRoot()->addChild(std::move(node));

        return room;
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