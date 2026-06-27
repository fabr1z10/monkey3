#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <any>

#include "tex.h"
#include "renderables/scumm_actor_info.h"

class Game;

template<typename T>
struct Loader;

template<>
struct Loader<Tex> {
	static std::shared_ptr<Tex> load(Game&, const std::string& path);
};

template<>
struct Loader<ScummActorInfo> {
	static std::shared_ptr<ScummActorInfo> load(Game&, const std::string& path);
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
	std::shared_ptr<T> get(const std::string& path) {
		auto& cache = getCache<T>();

		auto it = cache.find(path);
		if (it != cache.end())
			return it->second;

		auto asset = Loader<T>::load(_game, path);
		cache[path] = asset;
		return asset;
	}
private:
	std::unordered_map<std::type_index, std::any> caches;
	Game& _game;
};