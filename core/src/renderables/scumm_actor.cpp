#include "monkey3/renderables/scumm_actor.h"

ScummActor::ScummActor(std::shared_ptr<ScummActorInfo> info) : _info(info), _cycle(0){
	for (size_t i = 0; i <16; ++i) {
		_limb[i].limb_id = -1;
	}
}

void ScummActor::render(Renderer& r, glm::mat4 worldTransform) {
	glm::vec2 actor_pos = glm::vec2(worldTransform[3]);
	glm::vec2 x_move(-72, 100);
	glm::vec2 x_move_cur;
	glm::vec2 pos;
	_cycle++;
	for (auto & i : _limb) {
		if (i.limb_id != -1) {
			const auto &frame = _info->getFrame(i.limb_id, i.current);
			x_move_cur = x_move + glm::vec2(frame.relx, -frame.rely);
			x_move += glm::vec2(frame.movex, frame.movey);
			pos = actor_pos + x_move_cur;
			glm::vec2 bottomLeft = pos + glm::vec2(0.f, -frame.height);
			r.submitQuad(bottomLeft, glm::vec2(frame.width, frame.height), frame.uvCoords, glm::vec4(1.f), _info->getTexture());
			if ( _cycle > _info->getFrameDuration()) {
				i.current++;
				if (i.current > i.end) {
					i.current = i.start;
				}
			}
		}
	}
	if (_cycle > _info->getFrameDuration()) {
		_cycle = 0;
	}
}

void ScummActor::setAnimation(int animId) {

	_info->setAnimation(animId, _limb);
}