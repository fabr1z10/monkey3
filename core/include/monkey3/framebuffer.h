#pragma once

#include <GL/glew.h>

class FrameBuffer {
public:
	FrameBuffer() = default;

	~FrameBuffer();

	void bind() const;

	void unbind() const;

	GLuint texture() const { return _colorTexture; }

	int width() const { return _width; }
	int height() const { return _height; }
	void init(int width, int height);
private:
	GLuint _fbo = 0;
	GLuint _colorTexture = 0;
	GLuint _depthRbo = 0;

	int _width = 0;
	int _height = 0;
};