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