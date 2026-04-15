#include "monkey3/shader.h"
#include "monkey3/utils.h"
#include <string>
#include <stdexcept>


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexSrc = loadFile(vertexPath);
	std::string fragmentSrc = loadFile(fragmentPath);
	GLuint vid = 0;
	GLuint fid = 0;
	try {
		vid = compileShader(GL_VERTEX_SHADER, vertexSrc.c_str(), "vertex", vertexPath);//
		fid = compileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str(), "fragment", fragmentPath);
		_id = glCreateProgram();
		glAttachShader(_id, vid);
		glAttachShader(_id, fid);
		glLinkProgram(_id);
		GLint ok;
		glGetProgramiv(_id, GL_LINK_STATUS, &ok);
		if (!ok) {
			GLint len = 0;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &len);
			std::string log(len, '\0');
			glGetProgramInfoLog(_id, len, nullptr, log.data());
			throw std::runtime_error("Error linking program:\n" + log);
		}
	} catch(...) {
		if (vid) glDeleteShader(vid);
		if (fid) glDeleteShader(fid);
		if (_id) glDeleteProgram(_id);
		throw;
	}
	glDeleteShader(vid);
	glDeleteShader(fid);
}

Shader::~Shader() {
	if (_id) {
		glDeleteProgram(_id);
	}}

GLuint Shader::compileShader(GLenum type, const char* code, const char* stage, const std::string& fileName) {
	GLuint sid = glCreateShader(type);
	glShaderSource(sid, 1, &code, nullptr);
	glCompileShader(sid);

	GLint ok = 0;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		GLint len = 0;
		glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &len);
		std::string log(len, '\0');
		glGetShaderInfoLog(sid, len, nullptr, log.data());
		throw std::runtime_error(
				"Shader compile error\n"
				"File: " + fileName + "\n"
				"Stage: " + stage + "\n\n" +
				log
		);
	}
	return sid;
}

void Shader::use() const {
	glUseProgram(_id);
}

GLint Shader::getUniformLocation(const std::string &name) const {
	auto it = _uniformLocationCache.find(name);
	if (it == _uniformLocationCache.end()) {
		GLint location = glGetUniformLocation(_id, name.c_str());
		if (location == -1) {
			throw std::runtime_error("Uniform '" + name + "' not found in shader");
		}
		_uniformLocationCache[name] = location;
		return location;
	} else {
		return it->second;
	}

}

void Shader::setInt(const std::string &name, int value) {
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) {
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4& mat) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}


void Shader::setMat3(const std::string &name, const glm::mat3 &mat) {
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) {
	glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}
