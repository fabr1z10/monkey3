#include <yaml-cpp/yaml.h>
#include "monkey3/yaml_extension.h"
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

std::shared_ptr<SpriteInfo> Loader<SpriteInfo>::load(Game & game, const std::string &id) {
	auto& assetData = game.assetManager().getAssetData();
	if (!assetData["sprites"].IsDefined()) {
		throw std::runtime_error("No sprites defined! Sprites must reside in a <sprites> section in the yaml file.");
	}
	auto node = assetData["sprites"][id];
	if (!node.IsDefined()) {
		throw std::runtime_error("No sprite with id " + id + " defined!");
	}
	auto info = std::make_shared<SpriteInfo>();

	auto sheetFile = node["sheet"].as<std::string>();
	auto sheetPath = game.assetManager().getAssetDir() / sheetFile;
	size_t n = game.renderer().registerTexture(sheetPath.string());
	info->setTexture(n);
	auto* tex = game.renderer().getTexture(n);
	auto texSize = glm::vec2(tex->getWidth(), tex->getHeight());
	std::unordered_map<std::string, FrameData> frames;
	for (const auto& frame : node["frames"]) {
		FrameData data;
		auto uvCoordsPx = require<glm::ivec4>(frame.second, "quad");
		data.uvCoords.x = uvCoordsPx.x / texSize.x;
		data.uvCoords.y = uvCoordsPx.y / texSize.y;
		data.uvCoords.z = uvCoordsPx.z / texSize.x;
		data.uvCoords.w = uvCoordsPx.w / texSize.y;
		data.anchor = get<glm::vec2>(frame.second, "anchor", glm::vec2(0.f));
		data.size = get<glm::vec2>(frame.second, "size", glm::vec2(uvCoordsPx.z, uvCoordsPx.w));
		frames[frame.first.as<std::string>()] = data;
	}
	for (const auto& anim: node["animations"]) {
		auto animId = anim.first.as<std::string>();
		Animation animation;
		for (const auto& frame : anim.second["frames"]) {
			auto frameId = require<std::string>(frame, "id");
			auto cycles = get<int>(frame, "cycles", 1);
			animation.frames.push_back(frames.at(frameId));
			animation.frames.back().duration = cycles;
		}
		info->addAnimation(animId, animation);
	}
	return info;
}

std::shared_ptr<ScummActorInfo> Loader<ScummActorInfo>::load(Game& game, const std::string& id) {
	//std::filesystem::path yaml_path(path);
	auto& assetData = game.assetManager().getAssetData();
	if (!assetData["scumm_actors"].IsDefined()) {
		throw std::runtime_error("No scumm actors defined! Scumm actors must reside in a <scumm_actors> section in the yaml file.");
	}
	auto node = assetData["scumm_actors"][id];
	if (!node.IsDefined()) {
		throw std::runtime_error("No scumm actor with id " + id + " defined!");
	}
	auto sheetFile = node["sheet"].as<std::string>();
	auto sheetPath = game.assetManager().getAssetDir() / sheetFile;
	size_t n = game.renderer().registerTexture(sheetPath.string());
	auto info = std::make_shared<ScummActorInfo>();
	info->setTexture(n);
	info->setFrameDuration(20);
	std::cout << "Loaded texture with id = " << n << "\n";
	auto size = game.renderer().getTexture(n)->getWidth();
	std::cout << "tex size " << size << "\n";
	auto invSize = 1.f / size;
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