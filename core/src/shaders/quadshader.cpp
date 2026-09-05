#include <monkey3/shaders/quadshader.h>

QuadShader::QuadShader() : Shader("shaders/quad.vert", "shaders/quad.frag") {
	this->use();
	int samplers[16];
	for (int i = 0; i < 16; i++)
		samplers[i] = i;
	GLint loc = glGetUniformLocation(this->id(), "uTextures");
	glUniform1iv(loc, 16, samplers);
}