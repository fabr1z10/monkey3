#include "monkey3/game.h"
#include "monkey3/renderables/scumm_actor.h"
#include "monkey3/renderables/lines.h"
#include "monkey3/xmlroomfactory.h"
class TestRoomFactory : public RoomFactory {
public:
	using RoomFactory::RoomFactory;

    std::unique_ptr<Room> createRoom(Renderer& renderer) override {
        auto room = std::make_unique<Room>();
		//_game.assetManager().get<ScummActorInfo>( "assets/actors/dave.yamlload.cpp");

		//exit(2);

        size_t n = renderer.registerTexture("assets/images/kq1.png");

        auto node = std::make_unique<Node>();
        node->addRenderable(std::make_unique<QuadRenderable>(n, glm::vec4(30/1024.,1/1024.,28/1024.,40/1024.), glm::vec2(28,40)));
        room->getRoot()->addChild(std::move(node));

        return room;
    }
};

class TestActor : public RoomFactory {
public:
	using RoomFactory::RoomFactory;

	std::unique_ptr<Room> createRoom(Renderer& renderer) override {
		auto room = std::make_unique<Room>();
		auto scummActorInfo = _game.assetManager().get<ScummActorInfo>( "assets/actors/dave.yamlload.cpp");
		auto node = std::make_unique<Node>();
		auto actor = std::make_unique<ScummActor>(scummActorInfo);
		node->setPosition(glm::vec3(0, 0, 0));
		actor->setAnimation(10);
		actor->setAnimation(22);

		auto l = std::make_unique<Node>();
		l->addRenderable(std::make_unique<Lines>(std::vector<glm::vec2>{glm::vec2(0, 0), glm::vec2(100, 100)}, glm::vec4(1.f), false));


		node->addRenderable(std::move(actor));
		room->getRoot()->addChild(std::move(node));
		room->getRoot()->addChild(std::move(l));
		return room;
	}
};

int main() {
    Game g({320, 200}, {320, 200}, "Hello from Monkey!");
    g.setRoomFactory(std::make_unique<TestActor>(g));
    // need at least one pass, or render won't be called!
    g.addRenderPass(RenderPass(320, 200, 1));
    g.init();
    g.run();

}

