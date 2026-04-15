#include "monkey3/framebuffer.h"
#include <stdexcept>

FrameBuffer::~FrameBuffer() {
	if (_depthRbo) glDeleteRenderbuffers(1, &_depthRbo);
	if (_colorTexture) glDeleteTextures(1, &_colorTexture);
	if (_fbo) glDeleteFramebuffers(1, &_fbo);
}

void FrameBuffer::init(int width, int height) {
	_width = width;
	_height = height;

	// =========================
	// Create framebuffer
	// =========================
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	// =========================
	// Create color texture
	// =========================
	glGenTextures(1, &_colorTexture);
	glBindTexture(GL_TEXTURE_2D, _colorTexture);

	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 width,
				 height,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 nullptr);

	// filtering (important for pixel art!)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// attach texture to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER,
						   GL_COLOR_ATTACHMENT0,
						   GL_TEXTURE_2D,
						   _colorTexture,
						   0);

	// =========================
	// Create depth buffer
	// =========================
	glGenRenderbuffers(1, &_depthRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRbo);
	glRenderbufferStorage(GL_RENDERBUFFER,
						  GL_DEPTH24_STENCIL8,
						  width,
						  height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
							  GL_DEPTH_STENCIL_ATTACHMENT,
							  GL_RENDERBUFFER,
							  _depthRbo);

	// =========================
	// Check completeness
	// =========================
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Framebuffer is not complete!");
	}

	// unbind (important)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glViewport(0, 0, _width, _height);
}

void FrameBuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}