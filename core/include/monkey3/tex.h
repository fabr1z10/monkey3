#pragma once

#include <string>
#include <GL/glew.h>


class Tex {
public:
	bool loadFromFile(const std::string& path);

private:
	GLuint _id = 0;
	int _width = 0;
	int _height = 0;
};