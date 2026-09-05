#pragma once

#include "monkey3/renderpass.h"
#include "monkey3/shader.h"
#include "monkey3/batch.h"
#include "monkey3/primitive.h"

class IRenderItem {
public:
	virtual ~IRenderItem() = default;
	
	virtual void clear() = 0;
	
	virtual void init() = 0;
	
	virtual void render(const RenderPass& pass) = 0;

	virtual void submitPrimitive(const void* primitive) = 0;

};

template<typename Primitive, typename Vertex>
class RenderItem : public IRenderItem {
public:
	using PrimitiveType = Primitive;
	using VertexType = Vertex;

	RenderItem(std::unique_ptr<Shader> shader, int maxPrimitives) : _shader(std::move(shader)), _maxPrimitives(maxPrimitives) {}

	Batch<Vertex>& batch() {
		return _batch;
	}

	void clear() override {
		_batch.clear();
	}

	void init() override {
		glGenVertexArrays(1, &_batch.vao);
		glGenBuffers(1, &_batch.vbo);
		glGenBuffers(1, &_batch.ebo);
		
		glBindVertexArray(_batch.vao);
		glBindBuffer(GL_ARRAY_BUFFER, _batch.vbo);

		// VBO (allocate max size ONCE)
		glBufferData(GL_ARRAY_BUFFER,
			_maxPrimitives * Vertex::verticesPerPrimitive * sizeof(VertexType),
			nullptr,
			GL_DYNAMIC_DRAW);

		VertexTraits<Vertex>::setup();

		// EBO (allocate max size ONCE)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batch.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			_maxPrimitives * Vertex::indicesPerPrimitive * sizeof(uint32_t),
			nullptr,
			GL_DYNAMIC_DRAW);
	}

	void render(const RenderPass& pass) override {
		_shader->use();
		// setup view projection matrix
		const glm::mat4 vp = pass.camera->getViewProjectionMatrix();
		_shader->setMat4("uVP", vp);
		//for (int i = 0; i < _textures.size(); i++) {
		//	glActiveTexture(GL_TEXTURE0 + i);
		//	_textures[i]->bind();
		//}
		draw();
	}

	void draw() {
		glBindVertexArray(_batch.vao);
		glBindBuffer(GL_ARRAY_BUFFER, _batch.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, _batch.vertices.size() * sizeof(Vertex), _batch.vertices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _batch.ebo);

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _batch.indices.size() * sizeof(uint32_t), _batch.indices.data());
		glDrawElements(Vertex::primitiveType,
			_batch.indices.size(),
			GL_UNSIGNED_INT,
			nullptr);

	}


	void submitPrimitive(const void* primitive) override {
		submitPrimitiveTyped(
			*static_cast<const Primitive*>(primitive));
	}

private:
	virtual void submitPrimitiveTyped(const Primitive& primitive) = 0;

	int _maxPrimitives;
	std::unique_ptr<Shader> _shader;
	Batch<Vertex> _batch;

};


class QuadRenderItem : public RenderItem<QuadInfo, QuadVertex>
{
public:
	QuadRenderItem(int maxPrimitives);

	void submitPrimitiveTyped(const QuadInfo& info) override;
	
};

class LineRenderItem : public RenderItem<LineInfo, LineVertex> {
public:
	LineRenderItem(int maxPrimitives);

	void submitPrimitiveTyped(const LineInfo& info) override;


};

 

