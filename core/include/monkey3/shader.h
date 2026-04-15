#pragma once


#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <unordered_map>

class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	Shader(const Shader&) = delete;

	Shader& operator=(const Shader&) = delete;

	~Shader();

	void use() const;

	GLuint id() const { return _id; }

	void setInt(const std::string &name, int value);

	void setFloat(const std::string& name, float value);

	void setMat3(const std::string &name, const glm::mat3 &mat);

	void setMat4(const std::string &name, const glm::mat4 &mat);

	void setVec3(const std::string &name, const glm::vec3 &value);

	void setVec4(const std::string &name, const glm::vec4 &value);

	GLint getUniformLocation(const std::string& name) const;
private:
	GLuint compileShader(GLenum type, const char*, const char*, const std::string& fileName);

	GLuint _id = 0;

	mutable std::unordered_map<std::string, GLint> _uniformLocationCache;
};
