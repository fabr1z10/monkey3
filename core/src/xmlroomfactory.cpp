#include <monkey3/xmlroomfactory.h>

#include <iostream>
#include <monkey3/renderer.h>
#include "monkey3/adventureroom.h"
#include "monkey3/game.h"
#include <monkey3/renderables/quad.h>
#include <monkey3/yaml_extension.h>


XMLRoomFactory::XMLRoomFactory(Game &game) : RoomFactory(game) {
}




std::unique_ptr<Room> XMLRoomFactory::createRoom() {
	YAML::Node node = YAML::LoadFile(_filename);

	auto room_size = require<glm::ivec2>(node, "room_size");

	auto game_view = require<glm::ivec4>(node, "game_view");
	auto ui_view = require<glm::ivec4>(node, "ui_view");

	auto room = std::make_unique<AdventureRoom>(_game, room_size, game_view, ui_view);

	if (node["walk_area"]) {
		auto walkArea = std::make_unique<WalkArea>();

		for (const auto& areaNode : node["walk_area"]["areas"]) {
			auto poly = areaNode.as<Polygon>();
			walkArea->addArea(poly);
		}
		room->setWalkArea(std::move(walkArea));

	}

	auto nodes = node["nodes"];
	if (nodes) {
		for (const auto& n : nodes) {
			auto node = readNode(n);
			room->getRoot()->addChild(std::move(node));
		}
	}

//	auto n = std::make_unique<Node>();
//	auto quad = std::make_unique<Quad>(_game.renderer(), "assets/images/44.png");
//	n->addRenderable(std::move(quad));
//	room->getRoot()->addChild(std::move(n));
	room->setCameraPosition(640, 0);

	return room;
}

std::unique_ptr<Node> XMLRoomFactory::readNode(const YAML::Node &node) {
	auto n = std::make_unique<Node>();
	auto children = node["children"];
	if (children) {
		for (const auto& c : children) {
			auto childNode = readNode(c);
			n->addChild(std::move(childNode));
		}
	}
	auto renderable = node["renderable"];
	if (renderable) {
		auto r = readRenderable(renderable);
		if (r) {
			n->addRenderable(std::move(r));
		}
	}
	return std::move(n);
}

std::unique_ptr<Renderable> XMLRoomFactory::readRenderable(const YAML::Node &node) {
	auto type = require<std::string>(node, "type");
	if (type == "quad") {
		auto texture = require<std::string>(node, "image");
		auto anchor = get<glm::vec2>(node, "anchor", {0, 0});
		return std::make_unique<Quad>(_game.renderer(), texture, anchor);
	}
	return nullptr;
}