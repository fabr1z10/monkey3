#include <monkey3/renderables/quad.h>


Quad::Quad(int texId, const Tex* tex, glm::ivec4 coords, glm::vec2 anchor) : _anchor(anchor) {
	_texId = texId;
	_size = {tex->getWidth(), tex->getHeight()};
	if (coords == glm::ivec4()) {
		_coords = glm::vec4(0.f, 0.f, 1.f, 1.f);
	} else {
		_coords = {coords.x / _size[0],
				   coords.y / _size[1],
				   coords.z / _size[0],
				   coords.w / _size[1]};
	}
}


void Quad::render(Renderer& r, glm::mat4 worldTransform) {

	glm::vec2 pos = glm::vec2(worldTransform[3]);
	pos -= _anchor;
	r.submitQuad(pos, _size, _coords, glm::vec4(1.f), _texId);
}