#define STB_IMAGE_IMPLEMENTATION

#include "monkey3/tex.h"

#include <iostream>
#include <ostream>

#include "stb_image.h"

bool Tex::loadFromFile(const std::string &path) {
	int channels;
	unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &channels, 4);
	if (!data) {
		std::cerr << "Failed to load image: " << path << std::endl;
		std::cerr << "stb_image error: " << stbi_failure_reason() << std::endl;
		exit(1);
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	std::cout << "Loaded " << _width<< " x " << _height << " textures!" << std::endl;
	stbi_image_free(data);
	return false;
}

void Tex::bind() const {

	glBindTexture(GL_TEXTURE_2D, _id);
}
