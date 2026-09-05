#pragma once

#include <glm/glm.hpp>
#include <monkey3/color.h>

struct QuadInfo {

	QuadInfo() : position(0.f), anchor(0.f), size(0.f), uvRect(0.f), color(1.f), textureId(-1), paletteId(-1) {
	}

	QuadInfo(glm::vec3 position, glm::vec2 anchor, glm::vec2 size, glm::vec4 uvRect, glm::vec4 color, int textureId, int paletteId) :
		position(position), anchor(anchor), size(size), uvRect(uvRect), color(color), textureId(textureId), paletteId(paletteId) {
	}
	glm::vec3 position;
	glm::vec2 anchor;
	glm::vec2 size;
	glm::vec4 uvRect;
	glm::vec4 color;
	int textureId;
	int paletteId;

	static constexpr std::string_view name = "Quad";
};

struct LineInfo {
	LineInfo() : start(0.f), end(0.f), color(1.f) {
	}
	LineInfo(glm::vec2 startPoint, glm::vec2 endPoint, glm::vec4 color) : start(glm::vec3(startPoint, 0.f)),
		end(glm::vec3(endPoint, 0.f)), color(color) {}

	LineInfo(glm::vec3 start, glm::vec3 end, glm::vec4 color) : start(start), end(end), color(color) {}


	glm::vec3 start;
	glm::vec3 end;
	glm::vec4 color;

	static constexpr std::string_view name = "Line";

};