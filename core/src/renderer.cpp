#include "monkey3/renderer.h"
#include "monkey3/utils.h"
#include "monkey3/room.h"


RenderPass::RenderPass() {}
RenderPass::RenderPass(int width, int height, uint32_t mask) {
	camera = std::make_unique<OrthoCamera>(width, height, 0.1f, 100.f);
	viewport = {0, 0, width, height};
	layerMask = mask;
}

void Renderer::init(glm::ivec2 deviceSize) {

	_quadShader.emplace("assets/shaders/quad.vert", "assets/shaders/quad.frag");
	_quadShader->use();
	int samplers[16];
	for (int i = 0; i < 16; i++)
		samplers[i] = i;

	GLint loc = glGetUniformLocation(_quadShader->id(), "uTextures");
	glUniform1iv(loc, 16, samplers);

	_lineShader.emplace("assets/shaders/line.vert", "assets/shaders/line.frag");
	_screenShader.emplace("assets/shaders/screen.vert", "assets/shaders/screen.frag");
	_screenShader->use();
	_uSceneLoc = glGetUniformLocation(_screenShader->id(), "uScene");
	glUniform1i(_uSceneLoc, 0);


	initBatches();
	initScreenQuad();

	_frameBuffer.init(deviceSize.x, deviceSize.y);
}

void Renderer::setViewport(const glm::ivec4 &viewport) {
	_windowViewport = viewport;
}

void Renderer::beginFrame() {
	_frameBuffer.bind();   // also sets viewport to device size
	// 1. Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void Renderer::beginPass(const RenderPass& pass) {
	glViewport(pass.viewport.x, pass.viewport.y, pass.viewport.z, pass.viewport.w);
}

void Renderer::endPass(const RenderPass &) {
	// TODO empty for now
}

void Renderer::render(Room &room) {
	beginFrame();
	for (const auto &pass: _passes) {
		beginPass(pass);
		const glm::mat4 vp = pass.camera->getViewProjectionMatrix();

		_quadBatch.clear();
		_lineBatch.clear();
		room.render(*this, {pass.layerMask});
		// flush batches
		_quadShader->use();
		_quadShader->setMat4("uVP", vp);
		for (int i = 0; i < _textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			_textures[i].bind();
		}
		draw(_quadBatch, GL_TRIANGLES);
		_lineShader->use();
		_lineShader->setMat4("uVP", vp);
		draw(_lineBatch, GL_LINES);
		endPass(pass);
	}
	endFrame();
}

void Renderer::endFrame() {

	// =========================
	// 2. Switch back to screen
	// =========================
	_frameBuffer.unbind();

	// =========================
	// 3. Set window viewport (letterbox)
	// =========================
	glViewport(
		_windowViewport.x,
		_windowViewport.y,
		_windowViewport.z,
		_windowViewport.w
	);

	// =========================
	// 4. Clear screen (for black bars)
	// =========================
	glClear(GL_COLOR_BUFFER_BIT);

	// =========================
	// 5. Draw fullscreen quad with FBO texture
	// =========================
	_screenShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _frameBuffer.texture());

	// (optional but recommended)
	//GLint loc = glGetUniformLocation(_screenShader->id(), "uScene");
	//glUniform1i(loc, 0);
	glDisable(GL_DEPTH_TEST);
	drawScreenQuad();
	glEnable(GL_DEPTH_TEST);

	//drawScreenQuad();
}




void Renderer::initBatches() {
	initBatch(_quadBatch, _maxQuads * 4, _maxQuads * 6);
	initBatch(_lineBatch, _maxLines * 2, _maxLines * 2);
}

void Renderer::initScreenQuad() {
	// Fullscreen quad (2 triangles)
	float vertices[] = {
			// pos      // uv
			-1.f, -1.f,  0.f, 0.f,
			1.f, -1.f,  1.f, 0.f,
			1.f,  1.f,  1.f, 1.f,

			-1.f, -1.f,  0.f, 0.f,
			1.f,  1.f,  1.f, 1.f,
			-1.f,  1.f,  0.f, 1.f,
	};

	glGenVertexArrays(1, &_screenVao);
	glGenBuffers(1, &_screenVbo);

	glBindVertexArray(_screenVao);
	glBindBuffer(GL_ARRAY_BUFFER, _screenVbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// layout(location = 0) → vec2 pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	// layout(location = 1) → vec2 uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

void Renderer::drawScreenQuad() {
	glBindVertexArray(_screenVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Renderer::addRenderPass(RenderPass pass) {
	_passes.push_back(std::move(pass));
}

void Renderer::clearRenderPasses() {
	_passes.clear();
}

void Renderer::submitQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &uvRect, const glm::vec4 &color, int textureId) {
	// assumption: pos is the bottom left of the quad
	// uvrect gives: x, y = top left point in uv coords, z, w = size in uv coords
	QuadVertex bottomLeft;
	bottomLeft.pos = glm::vec3(pos.x, pos.y, 0.f);
	bottomLeft.uv = glm::vec2(uvRect.x, uvRect.y + uvRect.w);
	bottomLeft.color = color;
	bottomLeft.texIndex = textureId;

	QuadVertex bottomRight;
	bottomRight.pos = glm::vec3(pos.x + size.x, pos.y, 0.f);
	bottomRight.uv = glm::vec2(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	bottomRight.color = color;
	bottomRight.texIndex = textureId;

	QuadVertex topRight;
	topRight.pos = glm::vec3(pos.x + size.x, pos.y + size.y, 0.f);
	topRight.uv = glm::vec2(uvRect.x + uvRect.z, uvRect.y);
	topRight.color = color;
	topRight.texIndex = textureId;

	QuadVertex topLeft;
	topLeft.pos = glm::vec3(pos.x, pos.y + size.y, 0.f);
	topLeft.uv = glm::vec2(uvRect.x, uvRect.y);
	topLeft.color = color;
	topLeft.texIndex = textureId;

	auto index = _quadBatch.vertices.size();
	_quadBatch.vertices.push_back(bottomLeft);
	_quadBatch.vertices.push_back(bottomRight);
	_quadBatch.vertices.push_back(topRight);
	_quadBatch.vertices.push_back(topLeft);
	_quadBatch.indices.push_back(index);
	_quadBatch.indices.push_back(index+1);
	_quadBatch.indices.push_back(index+2);
	_quadBatch.indices.push_back(index+2);
	_quadBatch.indices.push_back(index+3);
	_quadBatch.indices.push_back(index);

}


void Renderer::registerTexture(const std::string &path) {
	assert(_textures.size() < MAX_TEXTURE_SLOTS && "Too many textures!");

	Tex tex;
	tex.loadFromFile(path);
	_textures.push_back(std::move(tex));

}

void Renderer::clear() {
	_textures.clear();
	_quadBatch.clear();
	_lineBatch.clear();

}
