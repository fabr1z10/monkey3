#include <monkey3/shapes/factory.h>
#include <monkey3/yaml_extension.h>
#include <monkey3/shapes/ellipse.h>

namespace shapes {

	void registerShape(const std::string &type, Creator c) {
		registry()[type] = std::move(c);
	}

	std::unordered_map<std::string, Creator> &registry() {
		static std::unordered_map<std::string, Creator> r;
		static bool initialized = false;

		if (!initialized) {
			initialized = true;

			registerShape("ellipse", readEllipse);

			registerShape("polygon", readPolygon);
		}

		return r;
	}

	std::unique_ptr<Shape> toShape(const YAML::Node &node) {
		auto type = require<std::string>(node, "type");
		auto it = registry().find(type);
		if (it == registry().end()) {
			throw std::runtime_error("Unknown shape type: " + type);
		}
		return it->second(node);
	}

	std::unique_ptr<Shape> readEllipse(const YAML::Node &node) {
		auto radii = require<glm::ivec2>(node, "radii");
		return std::make_unique<Ellipse>(radii.x, radii.y);
	}

	std::unique_ptr<Shape> readPolygon(const YAML::Node &node) {
		auto vertices = require<std::vector<glm::vec2>>(node, "vertices");
		return std::make_unique<Polygon>(vertices);
	}
}