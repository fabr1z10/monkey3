#pragma once

#include <cstdint>
#include <glm/glm.hpp>

struct Color
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 255;

	glm::vec4 toVec4() const {
		return glm::vec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	}
};

namespace Colors
{
	inline constexpr Color White {255, 255, 255};
	inline constexpr Color Black {0, 0, 0};
	inline constexpr Color Red   {255, 0, 0};
	inline constexpr Color Green {0, 255, 0};
	inline constexpr Color Blue  {0, 0, 255};
	inline constexpr Color Yellow  {255, 255, 0};
	inline constexpr Color Cyan    {0, 255, 255};
	inline constexpr Color Magenta {255, 0, 255};
	inline constexpr Color Transparent {0, 0, 0, 0};
}