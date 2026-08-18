#pragma once

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>
#include <monkey3/shapes/polygon.h>
#include "monkey3/utils.h"

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
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3 &rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& vec) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}
			try {
				vec.x = node[0].as<float>();
				vec.y = node[1].as<float>();
				vec.z = node[2].as<float>();
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

	template<>
	struct convert<Color>
	{
		static Node encode(const Color& c)
		{
			Node node;
			node.push_back(c.r);
			node.push_back(c.g);
			node.push_back(c.b);
			if (c.a != 255)
				node.push_back(c.a);
			return node;
		}

		static bool decode(const Node& node, Color& c)
		{
			if (node.IsSequence())
			{
				if (node.size() != 3 && node.size() != 4)
					return false;

				c.r = node[0].as<uint8_t>();
				c.g = node[1].as<uint8_t>();
				c.b = node[2].as<uint8_t>();
				c.a = node.size() == 4 ? node[3].as<uint8_t>() : 255;

				return true;
			}

			if (node.IsScalar())
			{
				return parseHexColor(node.as<std::string>(), c);
			}

			return false;
		}
	};
}