#include <yaml-cpp/yaml.h>
#include "monkey3/assetmanager.h"
#include <iostream>
#include <filesystem>
#include <glm/glm.hpp>
#include "monkey3/game.h"

std::shared_ptr<Tex> Loader<Tex>::load(Game&, const std::string& path) {
	auto tex = std::make_shared<Tex>();
	tex->loadFromFile(path);
	return tex;
}

std::shared_ptr<ScummActorInfo> Loader<ScummActorInfo>::load(Game& game, const std::string& path) {
	std::filesystem::path yaml_path(path);
	auto info = std::make_shared<ScummActorInfo>();
	YAML::Node node = YAML::LoadFile(path);
	auto sheetFile = node["sheet"].as<std::string>();
	auto sheetPath = yaml_path.parent_path() / sheetFile;
	size_t n = game.renderer().registerTexture(sheetPath);
	info->setTexture(n);
	info->setFrameDuration(20);
	std::cout << "Loaded texture with id = " << n << "\n";
	auto size = game.renderer().getTexture(n)->getWidth();
	std::cout << "tex size " << size << "\n";
	auto invSize = 1.f / size;
	std::filesystem::path sheet_path = yaml_path.parent_path() / sheetFile;
	for (const auto& frameNode : node["frames"]) {
		auto frameKey = frameNode.first.as<std::string>();
		int limb = std::stoi(frameKey.substr(0, frameKey.find("_")));
		ScummActorFrameData data;
		data.width = frameNode.second["width"].as<int>();
		data.height = frameNode.second["height"].as<int>();
		data.relx = frameNode.second["relx"].as<int>();
		data.rely = frameNode.second["rely"].as<int>();
		data.movex = frameNode.second["movex"].as<int>();
		data.movey = frameNode.second["movey"].as<int>();
		auto x = frameNode.second["x"].as<int>();
		auto y = frameNode.second["y"].as<int>();
		data.uvCoords = glm::vec4(x * invSize, y * invSize, data.width * invSize, data.height * invSize);
		info->addFrame(limb, data);

		//std::cout << frameKey << std::endl;
	}
	for (const auto& anim : node["animations"]) {
		ScummActorAnimation animation;
		int animId = anim.first.as<int>();
		for (const auto& limbNode : anim.second) {
			int limbId = limbNode.first.as<int>();
			animation.addLimb(limbId, limbNode.second["current"].as<int>(),
			        limbNode.second["start"].as<int>(), limbNode.second["end"].as<int>(), limbNode.second["frame"].as<int>());
		}
		info->addAnimation(animation);
	}
	std::unordered_map<int, int> codes;
	for (const auto& code : node["codes"]) {
		codes[code.first.as<int>()] = code.second.as<int>();
	}
	info->setCodes(codes);
	return info;
}