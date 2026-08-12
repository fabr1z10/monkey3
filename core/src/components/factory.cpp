#include <monkey3/components/factory.h>
#include <monkey3/yaml_extension.h>
#include <monkey3/components/hotspot.h>
#include <monkey3/shapes/factory.h>


namespace components {

	void registerComponent(const std::string &type, Creator c) {
		registry()[type] = std::move(c);
	}

	std::unordered_map<std::string, Creator> &registry() {
		static std::unordered_map<std::string, Creator> r;
		static bool initialized = false;

		if (!initialized) {
			initialized = true;

			registerComponent("hotspot", readHotspot);

		}

		return r;
	}

	std::unique_ptr<Component> toComponent(const YAML::Node &node) {
		auto type = require<std::string>(node, "type");
		auto it = registry().find(type);
		if (it == registry().end()) {
			throw std::runtime_error("Unknown shape type: " + type);
		}
		return it->second(node);
	}

	std::unique_ptr<Component> readHotspot(const YAML::Node &node) {
		auto shape = shapes::toShape(node["shape"]);
		return std::make_unique<HotSpot>(std::move(shape));
	}

}