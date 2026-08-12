#pragma once

#include <monkey3/component.h>
#include <yaml-cpp/yaml.h>

namespace components {

	using Creator = std::function<std::unique_ptr<Component>(const YAML::Node&)>;

	std::unordered_map<std::string, Creator>& registry();

	std::unique_ptr<Component> toShape(const YAML::Node& node);

	void registerComponent(const std::string& type, Creator c);

	std::unique_ptr<Component> readHotspot(const YAML::Node& node);


}