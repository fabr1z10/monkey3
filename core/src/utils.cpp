#include "monkey3/utils.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>


std::string loadFile(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + path);
	}

	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

bool parseHexColor(const std::string& str, Color& color)
{
	if (str.empty() || str[0] != '#')
		return false;

	if (str.size() != 7 && str.size() != 9)
		return false;

	auto hex = [](char c) -> int
	{
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'f')
			return c - 'a' + 10;
		if (c >= 'A' && c <= 'F')
			return c - 'A' + 10;
		return -1;
	};

	auto readByte = [&](size_t pos) -> int
	{
		int hi = hex(str[pos]);
		int lo = hex(str[pos + 1]);

		if (hi < 0 || lo < 0)
			return -1;

		return (hi << 4) | lo;
	};

	int r = readByte(1);
	int g = readByte(3);
	int b = readByte(5);
	int a = (str.size() == 9) ? readByte(7) : 255;

	if (r < 0 || g < 0 || b < 0 || a < 0)
		return false;

	color.r = static_cast<uint8_t>(r);
	color.g = static_cast<uint8_t>(g);
	color.b = static_cast<uint8_t>(b);
	color.a = static_cast<uint8_t>(a);

	return true;
}