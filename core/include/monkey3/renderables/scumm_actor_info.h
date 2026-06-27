#pragma once

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

struct ScummActorFrameData {
	int width;
	int height;
	int relx;
	int rely;
	int movex;
	int movey;
	glm::vec4 uvCoords;
};

struct Limb {
	int limb_id;
	int current;
	int start;
	int end;
	int frame;
};

class ScummActorAnimation {
public:
	ScummActorAnimation() = default;
	void addLimb(int limb_id, int current, int start, int end, int frame) {
		_limbs.push_back({limb_id, current, start, end, frame});
	}

	void set(Limb*);

private:
	std::vector<Limb> _limbs;
};

class ScummActorInfo {
public:
	ScummActorInfo() {
		_frames.resize(16);
	}

	void addFrame(int limb, const ScummActorFrameData& data) {
		_frames[limb].push_back(data);
	}

	void addAnimation(ScummActorAnimation animation) {
		_animations.push_back(animation);
	}

	void setTexture(int texId) {
		_texId = texId;
	}

	int getTexture() const {
		return _texId;
	}

	void setAnimation(size_t animationId, Limb* limb);

	void setCodes(const std::unordered_map<int, int>& codes) {
		_codeMap = codes;
	}

	int getFrameDuration() const;

	void setFrameDuration(int frameDuration);

	const ScummActorFrameData& getFrame(int limb, int frame) const;
private:
	std::vector<ScummActorAnimation> _animations;
	std::vector<std::vector<ScummActorFrameData>> _frames;
	std::unordered_map<int, int> _codeMap;
	int _texId;
	// frame duration in cycles
	int _frameDuration;
};

inline int ScummActorInfo::getFrameDuration() const {
	return _frameDuration;
}

inline void ScummActorInfo::setFrameDuration(int frameDuration) {
	_frameDuration = frameDuration;
}


class ScummActorInfoManager {
public:

};