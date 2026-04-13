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
