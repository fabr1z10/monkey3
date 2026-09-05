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
	auto pos = worldTransform[3];

	const auto& info = _info->getFrameData(_animation, _frame);
	
	QuadInfo quadInfo;

	quadInfo.position = pos;
	quadInfo.anchor = info.anchor;
	quadInfo.size = info.size;
	quadInfo.uvRect = info.uvCoords;
	quadInfo.color = glm::vec4(1.f);
	quadInfo.textureId = _info->getTexture();
	quadInfo.paletteId = 0;


	//r.submitQuad(quadInfo);

	_cycle++;
	if (_cycle > info.duration) {
		_cycle = 0;
		_frame = _info->nextFrame(_animation, _frame);
	}
}