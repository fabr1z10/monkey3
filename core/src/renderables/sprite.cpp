#include "monkey3/renderables/sprite.h"

Sprite::Sprite(std::shared_ptr<SpriteInfo> info) : _info(info) {
	setAnimation(info->getDefaultAnimation());
}

void Sprite::setAnimation(const std::string &animation) {

	if (_animation != animation) {
		_animation = animation;
		_frame = 0;
		_cycle = 0;
	}
}


void Sprite::render(Renderer& r, glm::mat4 worldTransform) {
	glm::vec2 pos = glm::vec2(worldTransform[3]);

	const auto& info = _info->getFrameData(_animation, _frame);
	pos -= info.anchor;

	r.submitQuad(pos, info.size, info.uvCoords, glm::vec4(1.f), _info->getTexture());

	_cycle++;
	if (_cycle > info.duration) {
		_cycle = 0;
		_frame = _info->nextFrame(_animation, _frame);
	}
}