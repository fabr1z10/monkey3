#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct FrameData {
	glm::vec4 uvCoords;
	glm::vec2 anchor;
	glm::vec2 size;
	int duration;
};


struct Animation {
	std::vector<FrameData> frames;
	int loopIndex = 0;
};

class SpriteInfo {
public:
	const FrameData& getFrameData(const std::string& animation, int frame) const;

	int nextFrame(const std::string& animation, int frame) const;

	void setTexture(int texId);

	int getTexture() const;

	void addAnimation(const std::string& animationId, const Animation& animation);

	std::string getDefaultAnimation() const;
private:
	int _texId;
	std::unordered_map<std::string, Animation> _animations;
	std::string _defaultAnimation;
};


inline void SpriteInfo::setTexture(int texId) {
	_texId = texId;
}

inline int SpriteInfo::getTexture() const {
	return _texId;
}

inline std::string SpriteInfo::getDefaultAnimation() const {
	return _defaultAnimation;
}