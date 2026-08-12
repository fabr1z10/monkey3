#pragma once

#include <monkey3/shape.h>
#include <yaml-cpp/yaml.h>

namespace shapes {

	using Creator = std::function<std::unique_ptr<Shape>(const YAML::Node&)>;

	std::unordered_map<std::string, Creator>& registry();

	std::unique_ptr<Shape> toShape(const YAML::Node& node);

	void registerShape(const std::string& type, Creator c);

	std::unique_ptr<Shape> readEllipse(const YAML::Node& node);

	std::unique_ptr<Shape> readPolygon(const YAML::Node& node);

	std::unique_ptr<Shape> readPoint(const YAML::Node& node);


}