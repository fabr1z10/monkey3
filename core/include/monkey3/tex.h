#pragma once

#include <string>
#include <GL/glew.h>


class Tex {
public:
	Tex() = default;

	Tex(const Tex&) = delete;

	Tex& operator=(const Tex&) = delete;

	Tex(Tex&& other) noexcept {
		_id = other._id;
		_width = other._width;
		_height = other._height;
		other._id = 0;
	}

	Tex& operator=(Tex&& other) noexcept {
		if (this != &other) {
			_id = other._id;
			_width = other._width;
			_height = other._height;
			other._id = 0;
		}
		return *this;
	}

	~Tex() {
		if (_id != 0)
			glDeleteTextures(1, &_id);
	}
	bool loadFromFile(const std::string& path);
	void bind() const;
	GLuint getId() const;

	int getWidth() const;

	int getHeight() const;
private:
	GLuint _id = 0;
	int _width = 0;
	int _height = 0;
};

inline GLuint Tex::getId() const {
	return _id;
}

inline int Tex::getWidth() const {
	return _width;
}

inline int Tex::getHeight() const {
	return _height;
}