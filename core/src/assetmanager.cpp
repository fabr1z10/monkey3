#include "monkey3/assetmanager.h"
#include <filesystem>

void AssetManager::setAssetFile(const std::string &path) {
	std::filesystem::path yaml_path(path);
	auto info = std::make_shared<ScummActorInfo>();
	_assetFile = YAML::LoadFile(path);
	std::filesystem::path p(path);
	_assetDir = p.parent_path();

}