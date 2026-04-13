#pragma once


#include <string>
#include <GL/glew.h>


class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	Shader(const Shader&) = delete;

	Shader& operator=(const Shader&) = delete;

	~Shader();

	void use() const;

	GLuint id() const { return _id; }

private:
	GLuint compileShader(GLenum type, const char*, const char*, const std::string& fileName);

	GLuint _id = 0;
};
