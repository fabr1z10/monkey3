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
	const glm::mat4 vp = pass.camera->getViewProjectionMatrix();
	_quadShader->use();
	_quadShader->setMat4("uVP", vp);
	_lineShader->use();
	_lineShader->setMat4("uVP", vp);

}

void Renderer::endPass(const RenderPass &) {
	// TODO empty for now
}

void Renderer::render(Room &room) {
	beginFrame();
	for (const auto &pass: _passes) {
		beginPass(pass);
		_quadBatch.clear();
		_lineBatch.clear();
		room.render(*this, {pass.layerMask});
		// flush batches
		draw(_quadBatch, _quadShader.value(), GL_TRIANGLES);
		draw(_lineBatch, _lineShader.value(), GL_LINES);
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

	drawScreenQuad();
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