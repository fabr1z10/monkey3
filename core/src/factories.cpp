#include <monkey3/factories.h>
#include <monkey3/yaml_extension.h>
#include <monkey3/renderables/quad.h>
#include <monkey3/shapes/point.h>
#include <monkey3/shapes/ellipse.h>
#include <monkey3/game.h>
#include "monkey3/renderables/sprite.h"


std::unique_ptr<Renderable> readQuad(Renderer& r, const YAML::Node& node) {
	auto texture = require<std::string>(node, "image");

	auto coords = get<glm::ivec4>(node, "coords", glm::ivec4());

	auto anchor = get<glm::vec2>(node, "anchor", {0, 0});

	auto texId = r.registerTexture(texture);
	const auto* tex = r.getTexture(texId);
	return std::make_unique<Quad>(texId, tex, coords, anchor);
}

std::unique_ptr<Renderable> readPoint(Renderer& r, const YAML::Node& node) {
	auto position = require<glm::vec2>(node, "position");
	auto color =  get<Color>(node, "color", Colors::White);
	auto point = std::make_unique<shapes::Point>(position.x, position.y);
	point->setColor(color);
	return point;
}

std::unique_ptr<Renderable> readSprite(Renderer& r, const YAML::Node& node) {
	auto id = node["id"].as<std::string>();
	auto spriteInfo = r.getGame().assetManager().get<SpriteInfo>(id);
	auto sprite = std::make_unique<Sprite>(spriteInfo);
	return sprite;
}

std::unique_ptr<Renderable> readPolygon(Renderer& r, const YAML::Node& node) {
	auto vertices = require<std::vector<glm::vec2>>(node, "vertices");
	auto color =  get<Color>(node, "color", Colors::White);

	auto polygon = std::make_unique<shapes::Polygon>(vertices);
	polygon->setColor(color);
	return polygon;
}

std::unique_ptr<Renderable> readEllipse(Renderer& r, const YAML::Node& node) {
	auto rx = require<float>(node, "rx");
	auto ry = require<float>(node, "ry");
	auto color =  get<Color>(node, "color", Colors::White);

	auto ellipse = std::make_unique<shapes::Ellipse>(rx, ry);
	ellipse->setColor(color);
	return ellipse;
}