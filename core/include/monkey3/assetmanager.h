#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <any>
#include <yaml-cpp/yaml.h>
#include <filesystem>

#include "tex.h"
#include "renderables/scumm_actor_info.h"
#include "renderables/sprite_info.h"

class Game;

template<typename T>
struct Loader;

template<>
struct Loader<Tex> {
	static std::shared_ptr<Tex> load(Game&, const std::string& path);
};

template<>
struct Loader<ScummActorInfo> {
	static std::shared_ptr<ScummActorInfo> load( Game&, const std::string& id);
};

template<>
struct Loader<SpriteInfo> {
	static std::shared_ptr<SpriteInfo> load(Game&, const std::string& id);
};

class AssetManager {
public:
	AssetManager(Game& game) : _game(game) {}

	template<typename T>
	std::unordered_map<std::string, std::shared_ptr<T>>& getCache()
	{
		auto& anyCache = caches[typeid(T)];

		if (!anyCache.has_value()) {
			anyCache = std::unordered_map<std::string, std::shared_ptr<T>>{};
		}

		return std::any_cast<
				std::unordered_map<std::string, std::shared_ptr<T>>&
		>(anyCache);
	}

	template<typename T>
	std::shared_ptr<T> get(const std::string& id) {
		auto& cache = getCache<T>();

		auto it = cache.find(id);
		if (it != cache.end())
			return it->second;

		auto asset = Loader<T>::load(_game, id);
		cache[id] = asset;
		return asset;
	}

	void setAssetFile(const std::string& path);

	const YAML::Node& getAssetData() const;

	std::filesystem::path getAssetDir() const;
private:
	std::unordered_map<std::type_index, std::any> caches;
	Game& _game;
	YAML::Node _assetFile;
	std::filesystem::path _assetDir;
};

inline const YAML::Node &AssetManager::getAssetData() const {

	return _assetFile;
}

inline std::filesystem::path AssetManager::getAssetDir() const {

	return _assetDir;
}