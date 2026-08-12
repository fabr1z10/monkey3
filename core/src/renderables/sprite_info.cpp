#include "monkey3/renderables/sprite_info.h"
#include <stdexcept>

void SpriteInfo::addAnimation(const std::string &animationId, const Animation &animation) {
	if (_animations.empty()) _defaultAnimation = animationId;
	_animations[animationId] = animation;

}


const FrameData &SpriteInfo::getFrameData(const std::string &animation, int frame) const {

	auto it = _animations.find(animation);
	if (it == _animations.end()) {
		throw std::runtime_error("Animation not found: " + animation);
	}
	const Animation& anim = it->second;
	if (frame < 0 || frame >= anim.frames.size()) {
		throw std::runtime_error("Frame index out of bounds for animation: " + animation);
	}
	return anim.frames[frame];
}

int SpriteInfo::nextFrame(const std::string &animation, int frame) const {

	auto it = _animations.find(animation);
	if (it == _animations.end()) {
		throw std::runtime_error("Animation not found: " + animation);
	}
	const Animation& anim = it->second;
	if (frame < 0 || frame >= anim.frames.size()) {
		throw std::runtime_error("Frame index out of bounds for animation: " + animation);
	}
	int nextFrame = frame + 1;
	if (nextFrame >= anim.frames.size()) {
		nextFrame = anim.loopIndex; // Loop back to the loop index
	}
	return nextFrame;
}