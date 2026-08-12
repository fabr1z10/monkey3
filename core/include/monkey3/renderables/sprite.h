#pragma once

#include "monkey3/renderable.h"
#include "monkey3/renderables/sprite_info.h"

class Sprite : public Renderable {

public:
	Sprite(std::shared_ptr<SpriteInfo> info);

	void render(Renderer&, glm::mat4 worldTransform) override;

	void setAnimation(const std::string& animation);
private:
	std::shared_ptr<SpriteInfo> _info;
	std::string _animation;
	int _frame;
	int _cycle = 0;
};