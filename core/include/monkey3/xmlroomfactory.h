#pragma once

#include "monkey3/roomfactory.h"
#include <yaml-cpp/yaml.h>

class XMLRoomFactory : public RoomFactory {
public:
	XMLRoomFactory(Game &game);

	void setFilename(const std::string& filename) { _filename = filename; }

	std::unique_ptr<Room> createRoom() override;
private:
	std::unique_ptr<Node> readNode(const YAML::Node& node);

	std::unique_ptr<Renderable> readRenderable(const YAML::Node& node);
	//void readRenderPasses(const YAML::Node&);

	std::string _filename;
};