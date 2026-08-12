#pragma once


#include <yaml-cpp/yaml.h>
#include <monkey3/renderable.h>
#include <monkey3/component.h>


std::unique_ptr<Renderable> readQuad(Renderer&, const YAML::Node& node);
std::unique_ptr<Renderable> readPoint(Renderer&, const YAML::Node& node);
std::unique_ptr<Renderable> readPolygon(Renderer&, const YAML::Node& node);
std::unique_ptr<Renderable> readEllipse(Renderer&, const YAML::Node& node);
std::unique_ptr<Renderable> readSprite(Renderer&, const YAML::Node& node);


// components
std::unique_ptr<Component> readHotSpot(const YAML::Node& node);
