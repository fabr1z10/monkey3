#pragma once


#include <yaml-cpp/yaml.h>
#include <monkey3/renderable.h>
#include <monkey3/component.h>


std::unique_ptr<Renderable> readQuad(Game&, const YAML::Node& node);
std::unique_ptr<Renderable> readPoint(Game&, const YAML::Node& node);
std::unique_ptr<Renderable> readPolygon(Game&, const YAML::Node& node);
std::unique_ptr<Renderable> readEllipse(Game&, const YAML::Node& node);
std::unique_ptr<Renderable> readSprite(Game&, const YAML::Node& node);


// components
std::unique_ptr<Component> readHotSpot(Game&, const YAML::Node& node);
std::unique_ptr<Component> readPlayer2D(Game&, const YAML::Node& node);