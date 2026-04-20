#pragma once

#include <array>
#include "glm/glm.hpp"
#include "vertex.h"
#include "batch.h"
#include "shader.h"
#include "monkey3/framebuffer.h"
#include <optional>
#include <memory>
#include "monkey3/camera.h"
#include "monkey3/tex.h"


class Room;

static constexpr int MAX_TEXTURE_SLOTS = 16;

using RenderLayerMask = uint32_t;


struct RenderPass {
	RenderPass();
	RenderPass(int width, int height, uint32_t mask);

	std::unique_ptr<Camera> camera = nullptr;
	glm::ivec4 viewport{0, 0, 0, 0};
	uint32_t layerMask = 0xFFFFFFFF;
};

struct RenderContext {
	uint32_t layerMask;
};

class Renderer {

public:

	void init(glm::ivec2 deviceSize);

	void clear();

	void beginFrame();   // clear + reset batches
	void beginPass(const RenderPass&);
	void render(Room& room);

	void endPass(const RenderPass&);
	void endFrame();     // flush to GPU

	void initBatches();

	void submitQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& uvRect, const glm::vec4& color, int textureId);

	void submitLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

	void setViewport(const glm::ivec4& viewport);

	void addRenderPass(RenderPass pass);

	void clearRenderPasses();

	void registerTexture(const std::string& path);
private:
	template<typename T>
	void draw(Batch<T>& batch, GLenum mode) {
		glBindVertexArray(batch.vao);
		// =========================
		// CPU → GPU upload (SUBDATA)
		// =========================
		glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batch.vertices.size() * sizeof(T), batch.vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch.indices.size() * sizeof(uint32_t), batch.indices.data());

		glDrawElements(mode,
			batch.indices.size(),
			GL_UNSIGNED_INT,
			nullptr);
	}

	template<typename T>
	void initBatch(Batch<T>& batch, int maxVertices, int maxIndices) {
		glGenVertexArrays(1, &batch.vao);
		glGenBuffers(1, &batch.vbo);
		glGenBuffers(1, &batch.ebo);
		glBindVertexArray(batch.vao);
		glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);

		// VBO (allocate max size ONCE)
		glBufferData(GL_ARRAY_BUFFER,
					 maxVertices * sizeof(T),
					 nullptr,
					 GL_DYNAMIC_DRAW);

		VertexTraits<T>::setup();

		// EBO (allocate max size ONCE)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 maxIndices * sizeof(uint32_t),
					 nullptr,
					 GL_DYNAMIC_DRAW);
	}

	void initScreenQuad();
	void drawScreenQuad();
	Batch<QuadVertex> _quadBatch;
	Batch<LineVertex> _lineBatch;
	std::optional<Shader> _quadShader;
	std::optional<Shader> _lineShader;
	int _maxQuads = 1000;
	int _maxLines = 1000;
	FrameBuffer _frameBuffer;
	glm::ivec4 _windowViewport;

	// screen pass
	std::optional<Shader> _screenShader;
	GLuint _screenVao = 0;
	GLuint _screenVbo = 0;

	GLint _uSceneLoc = -1;

	std::vector<RenderPass> _passes;

	std::vector<Tex> _textures;
};