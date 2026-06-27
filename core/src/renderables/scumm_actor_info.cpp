#include "monkey3/renderables/scumm_actor_info.h"

const ScummActorFrameData &ScummActorInfo::getFrame(int limb, int frame) const {
	auto code = _codeMap.at(frame);
	return _frames[limb][code];
}

void ScummActorInfo::setAnimation(size_t animationId, Limb* limb) {
	_animations[animationId].set(limb);
}

void ScummActorAnimation::set(Limb * limb) {
	for (auto& l : _limbs) {
		limb[l.limb_id] = l;
	}
}