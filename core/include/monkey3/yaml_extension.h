#pragma once

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>
#include <monkey3/math/walkarea_classes.h>

template<typename T>
T require(const YAML::Node& node, const std::string& key)
{
	if (!node[key])
		throw std::runtime_error("Missing required YAML field: " + key);

	try {
		return node[key].as<T>();
	}
	catch (const YAML::Exception& e) {
		throw std::runtime_error(
				"Invalid YAML field '" + key + "': " + e.what()
		);
	}
}

template<typename T>
T get(const YAML::Node& node, const std::string& key, const T& defaultValue)
{
	if (!node[key])
		return defaultValue;

	try {
		return node[key].as<T>();
	}
	catch (const YAML::Exception& e) {
		throw std::runtime_error(
				"Invalid YAML field '" + key + "': " + e.what()
		);
	}
}

namespace YAML {

	template <>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& vec) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}
			try {
				vec.x = node[0].as<float>();
				vec.y = node[1].as<float>();
			}
			catch (const YAML::Exception& e) {
				return false;
			}
			return true;
		}
	};

	template <>
	struct convert<glm::ivec2> {
		static Node encode(const glm::ivec2 &rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, glm::ivec2& vec) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}
			try {
				vec.x = node[0].as<int>();
				vec.y = node[1].as<int>();
			}
			catch (const YAML::Exception& e) {
				return false;
			}
			return true;
		}
	};

	template <>
	struct convert<glm::ivec4> {
		static Node encode(const glm::ivec4 &rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::ivec4& vec) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}
			try {
				vec.x = node[0].as<int>();
				vec.y = node[1].as<int>();
				vec.z = node[2].as<int>();
				vec.w = node[3].as<int>();
			}
			catch (const YAML::Exception& e) {
				return false;
			}
			return true;
		}
	};

	template <>
	struct convert<Polygon> {
		static Node encode(const Polygon& rhs) {
			Node node;
			node["outer"] = rhs.outer;
			node["holes"] = rhs.holes;
			return node;
		}

		static bool decode(const Node& node, Polygon& poly) {
			if (!node.IsMap()) {
				return false;
			}
			try {
				poly.outer = node["outer"].as<std::vector<glm::vec2>>();
				if (node["holes"]) {
					for (const auto& hole : node["holes"]) {
						poly.holes.push_back(hole.as<std::vector<glm::vec2>>());
					}
				}
			}
			catch (const YAML::Exception& e) {
				return false;
			}
			return true;
		}
	};

}