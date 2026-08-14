#include <monkey3/xmlroomfactory.h>

#include <iostream>
#include <monkey3/renderer.h>
#include "monkey3/adventureroom.h"
#include "monkey3/game.h"
#include <monkey3/renderables/quad.h>
#include <monkey3/yaml_extension.h>
#include <monkey3/factories.h>
#include <monkey3/services/collisionengine.h>

XMLRoomFactory::XMLRoomFactory(Game &game) : RoomFactory(game) {

	_renderableFactories["quad"] = readQuad;
	_renderableFactories["point"] = readPoint;
	_renderableFactories["polygon"] = readPolygon;
	_renderableFactories["ellipse"] = readEllipse;
	_renderableFactories["sprite"] = readSprite;

	_componentFactories["player2D"] = readPlayer2D;
	_componentFactories["collider"] = readCollider;


	_roomFactories["world"] = readWorldRoom;
	_roomFactories["adventure"] = readAdventureRoom;

	//_componentFactories["hotspot"] = readHotSpot;

}




std::unique_ptr<Room> XMLRoomFactory::createRoom() {
	YAML::Node node = YAML::LoadFile((_game.getHomeDir() / _filename).string());

	auto type = require<std::string>(node, "type");

	auto itRoomFactory = _roomFactories.find(type);
	if (itRoomFactory == _roomFactories.end()) {
		throw std::runtime_error("Unknown room type: " + type);
	}
	auto room = itRoomFactory->second(_game, node);

	if (auto collisionEngineNode = node["collision_engine"]) {
		auto size = require<glm::ivec2>(collisionEngineNode, "size");
		//auto collisionEngine = std::make_unique<CollisionEngine2D>(glm::vec3(size, 0.f));
		room->addService<CollisionEngine2D>(glm::vec3(size, 0.f));

	}

	auto nodes = node["nodes"];
	if (nodes) {
		for (const auto& n : nodes) {
			auto node = readNode(n);
			auto pos = get<glm::vec2>(n, "position", glm::vec2(0.f));
			node->setPosition(glm::vec3(pos, 0));
			room->getRoot()->addChild(std::move(node));
		}
	}

//	auto n = std::make_unique<Node>();
//	auto quad = std::make_unique<Quad>(_game.renderer(), "data/images/44.png");
//	n->addRenderable(std::move(quad));
//	room->getRoot()->addChild(std::move(n));
	//room->setCameraPosition(640, 0);

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
		n->addRenderable(readRenderable(_game, renderable));
	}

	for (const auto& component : node["components"]) {
		auto c = readComponent(component);
		if (c) {
			n->addComponent(std::move(c));
		}
	}

	

	return std::move(n);
}

//std::unique_ptr<Renderable> XMLRoomFactory::readRenderable(const YAML::Node &node) {
//	auto type = require<std::string>(node, "type");
//
//	auto factoryIter = _renderableFactories.find(type);
//	if (factoryIter != _renderableFactories.end()) {
//		return factoryIter->second(_game, node);
//	} else {
//		throw std::runtime_error("Unknown renderable type: " + type);
//	}
//	return nullptr;
//}

std::unique_ptr<Component> XMLRoomFactory::readComponent(const YAML::Node &node) {
	auto type = require<std::string>(node, "type");
	auto compIter = _componentFactories.find(type);
	if (compIter != _componentFactories.end()) {
		return compIter->second(_game, node);
	}
	else {
		std::cout << "SUCA\n";

		throw std::runtime_error("Unknown component: " + type);
	}
	return nullptr;

}