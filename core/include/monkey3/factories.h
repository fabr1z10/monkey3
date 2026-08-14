#pragma once


#include <yaml-cpp/yaml.h>
#include <monkey3/renderable.h>
#include <monkey3/component.h>
#include <monkey3/shape.h>

class Room;


template<typename T>
using FactoryMethod = std::function<std::unique_ptr<T>(Game&, const YAML::Node&)>;


const std::unordered_map < std::string, FactoryMethod<shapes::Shape> >& shapeFactories();

const std::unordered_map < std::string, FactoryMethod<Renderable> >& renderableFactories();


std::unique_ptr<shapes::Shape> readShape(Game&, const YAML::Node& node);

std::unique_ptr<Renderable> readRenderable(Game&, const YAML::Node& node);




// shape factories
std::unique_ptr<shapes::Shape> readBox(Game&, const YAML::Node& node);
std::unique_ptr<shapes::Shape> readPoint(Game&, const YAML::Node& node);
std::unique_ptr<shapes::Shape> readPolygon(Game&, const YAML::Node& node);
std::unique_ptr<shapes::Shape> readEllipse(Game&, const YAML::Node& node);

// renderable factories
std::unique_ptr<Renderable> readQuad(Game&, const YAML::Node& node);
std::unique_ptr<Renderable> readSprite(Game&, const YAML::Node& node);


// components
std::unique_ptr<Component> readHotSpot(Game&, const YAML::Node& node);
std::unique_ptr<Component> readPlayer2D(Game&, const YAML::Node& node);
std::unique_ptr<Component> readCollider(Game&, const YAML::Node& node);

// room
std::unique_ptr<Room> readWorldRoom(Game&, const YAML::Node& node);
std::unique_ptr<Room> readAdventureRoom(Game&, const YAML::Node& node);