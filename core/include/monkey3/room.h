#pragma once

#include "monkey3/renderer.h"
#include "monkey3/node.h"
#include "monkey3/service.h"
#include "service.h"
#include <typeindex>

class Game;




class Room {
	// TODO!!!

public:
	Room(Game&);
	virtual ~Room() = default;
	virtual void update(float dt);
	virtual void render(const RenderContext&);
	Node* getRoot();

	template<typename T, typename... Args>
	T& addService(Args&&... args)
	{
		static_assert(std::is_base_of<IService, T>::value,
					  "Service must derive from IService");

		auto service = std::make_unique<T>(std::forward<Args>(args)...);

		T* ptr = service.get();
		_services[typeid(T)] = std::move(service);

		return *ptr;
	}

	template<typename T>
	T* getService() const
	{
		auto it = _services.find(typeid(T));
		if (it == _services.end())
			return nullptr;

		return static_cast<T*>(it->second.get());
	}

	template<typename T>
	bool hasService() const
	{
		return _services.find(typeid(T)) != _services.end();
	}

private:
	std::unique_ptr<Node> _root;
	std::unordered_map<std::type_index, std::unique_ptr<IService>> _services;

protected:
	Game& _game;
	Renderer& _renderer;
};

inline Node *Room::getRoot() {

	return _root.get();
}
