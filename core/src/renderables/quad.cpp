#include <monkey3/renderables/quad.h>


Quad::Quad(Renderer& r, const std::string &texture, glm::vec2 anchor) : _anchor(anchor) {
	_texId = r.registerTexture(texture);
	auto tex = r.getTexture(_texId);
	_size = {tex->getWidth(), tex->getHeight()};
}
void Quad::render(Renderer& r, glm::mat4 worldTransform) {

	glm::vec2 pos = glm::vec2(worldTransform[3]);
	pos -= _anchor;
	r.submitQuad(pos, _size, glm::vec4(0,0,1,1), glm::vec4(1.f), _texId);
}