#pragma once

#include "monkey3/renderable.h"
#include "monkey3/renderables/scumm_actor_info.h"




class ScummActor : public Renderable {
public:
	ScummActor(std::shared_ptr<ScummActorInfo>);

	void render(Renderer&, glm::mat4 worldTransform) override;

	void setAnimation(int animId);
private:
	int _cycle;
	Limb _limb[16];
	std::shared_ptr<ScummActorInfo> _info;

};