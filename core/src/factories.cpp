#include <monkey3/factories.h>
#include <monkey3/yaml_extension.h>
#include <monkey3/renderables/quad.h>

#include <monkey3/shapes/point.h>
#include <monkey3/shapes/ellipse.h>
#include <monkey3/shapes/box.h>


#include <monkey3/game.h>
#include "monkey3/renderables/sprite.h"

#include <monkey3/components/playerwalk2d.h>
#include <monkey3/components/collider.h>
#include <monkey3/adventureroom.h>
#include <monkey3/worldroom.h>

const std::unordered_map<std::string, FactoryMethod<shapes::Shape> >& shapeFactories() {
	static const std::unordered_map<std::string, FactoryMethod<shapes::Shape> > factories {
		{ "box",     readBox },
		{ "ellipse", readEllipse },
		{ "polygon", readPolygon }
	};

	return factories;
}

const std::unordered_map<std::string, FactoryMethod<Renderable> >& renderableFactories() {
	static std::unordered_map<std::string, FactoryMethod<Renderable> > factories{
		{ "quad", readQuad },
		{ "point", readPoint },
		{ "polygon", readPolygon },
		{ "ellipse", readEllipse },
		{ "sprite", readSprite }
	};
	return factories;
}

std::unique_ptr<shapes::Shape> readShape(Game& game, const YAML::Node& node) {
	auto type = require<std::string>(node, "type");
	auto it = shapeFactories().find(type);
	if (it == shapeFactories().end()) {
		throw std::runtime_error("Unknown shape type: " + type);
	}
	return it->second(game, node);
}

std::unique_ptr<Renderable> readRenderable(Game& game, const YAML::Node& node) {
	auto type = require<std::string>(node, "type");
	auto it = renderableFactories().find(type);
	if (it == renderableFactories().end()) {
		throw std::runtime_error("Unknown renderable type: " + type);
	}
	return it->second(game, node);
}


std::unique_ptr<Renderable> readQuad(Game& game, const YAML::Node& node) {
	auto texture = require<std::string>(node, "image");

	auto coords = get<glm::ivec4>(node, "coords", glm::ivec4());

	auto anchor = get<glm::vec2>(node, "anchor", {0, 0});

	auto& r = game.renderer();
	auto texId = r.registerTexture(texture);
	const auto* tex = r.getTexture(texId);
	return std::make_unique<Quad>(texId, tex, coords, anchor);
}

std::unique_ptr<shapes::Shape> readPoint(Game& game, const YAML::Node& node) {
	auto position = require<glm::vec2>(node, "position");
	auto color = get<Color>(node, "color", Colors::White).toVec4();
	auto point = std::make_unique<shapes::Point>(position.x, position.y);
	point->setColor(color);
	return point;
}

std::unique_ptr<Renderable> readSprite(Game& game, const YAML::Node& node) {
	auto id = node["id"].as<std::string>();
	auto spriteInfo = game.assetManager().get<SpriteInfo>(id);
	auto sprite = std::make_unique<Sprite>(spriteInfo);
	return sprite;
}

std::unique_ptr<shapes::Shape> readBox(Game& game, const YAML::Node& node) {
	auto width = require<float>(node, "width");
	auto height = require<float>(node, "height");
	auto anchor = get<glm::vec2>(node, "anchor", glm::vec2(0.f));
	auto color = get<Color>(node, "color", Colors::White).toVec4();
	auto box = std::make_unique<shapes::Box>(width, height, anchor);
	box->setColor(color);
	return box;
}

std::unique_ptr<shapes::Shape> readPolygon(Game& game, const YAML::Node& node) {
	auto vertices = require<std::vector<glm::vec2>>(node, "vertices");
	auto color = get<Color>(node, "color", Colors::White).toVec4();

	auto polygon = std::make_unique<shapes::Polygon>(vertices);
	polygon->setColor(color);
	return polygon;
}

std::unique_ptr<shapes::Shape> readEllipse(Game& game, const YAML::Node& node) {
	auto rx = require<float>(node, "rx");
	auto ry = require<float>(node, "ry");
	auto color = get<Color>(node, "color", Colors::White).toVec4();

	auto ellipse = std::make_unique<shapes::Ellipse>(rx, ry);
	ellipse->setColor(color);
	return ellipse;
}

std::unique_ptr<Component> readCollider(Game& game, const YAML::Node& node) {
	auto shape = readShape(game, node["shape"]);
	auto flag = require<int>(node, "flag");
	auto mask = require<int>(node, "mask");
	auto tag = require<int>(node, "tag");
	return std::make_unique<SimpleCollider>(std::move(shape), flag, mask, tag);
}

std::unique_ptr<Component> readPlayer2D(Game& game, const YAML::Node& node) {
	ControllerInfo info;
	info.size = require<glm::vec3>(node, "size");
	info.maxSpeed = require<float>(node, "maxSpeed");
	info.accelerationTime = require<float>(node, "accelerationTime");
	info.jumpHeight = require<float>(node, "jumpHeight");
	info.timeToJumpApex = require<float>(node, "timeToJumpApex");
	info.maskDown = get<int>(node, "maskDown", 2);
	info.maskUp = get<int>(node, "maskDown", 2 | 32);
	info.horizontalRayCount = get<int>(node, "horizontalRayCount", 4);
	info.verticalRayCount = get<int>(node, "verticalRayCount", 4);
	info.skinWidth = get<float>(node, "skinWidth", .015f);
	info.maxClimbAngle = glm::radians(get<float>(node, "maxClimbAngle", 80.f));
	info.maxDescendAngle = glm::radians(get<float>(node, "maxDescendAngle", 80.f));

	return std::make_unique<PlayerWalk2D>(game, info);
}

std::unique_ptr<Room> readAdventureRoom(Game& game, const YAML::Node& node) {
	auto room_size = require<glm::ivec2>(node, "room_size");
	auto game_view = require<glm::ivec4>(node, "game_view");
	auto ui_view = require<glm::ivec4>(node, "ui_view");

	auto room = std::make_unique<AdventureRoom>(game, room_size, game_view, ui_view);

	if (node["walk_area"]) {
		auto walkArea = std::make_unique<WalkArea>();

		for (const auto& areaNode : node["walk_area"]["areas"]) {
			Area area{ -1, areaNode["outer"].as<std::vector<glm::vec2>>(), {}, {} }; // {};
			for (const auto& holeNode : areaNode["holes"]) {
				area.holes.emplace_back(holeNode.as<std::vector<glm::vec2>>());
			}
			walkArea->addArea(area);
		}
		room->setWalkArea(std::move(walkArea));

	}
	return std::move(room);
}

std::unique_ptr<Room> readWorldRoom(Game& game, const YAML::Node& node) {
	auto room_size = require<glm::ivec2>(node, "room_size");
	auto game_view = require<glm::ivec4>(node, "game_view");
	auto ui_view = require<glm::ivec4>(node, "ui_view");

	auto room = std::make_unique<WorldRoom>(game, room_size, game_view, ui_view);
	return std::move(room);
}


