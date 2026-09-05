#pragma once

#include <memory>
#include <monkey3/camera.h>


struct RenderPass {
	RenderPass();
	RenderPass(int width, int height, uint32_t mask);

	std::unique_ptr<OrthoCamera> camera = nullptr;
	glm::ivec4 viewport{ 0, 0, 0, 0 };

	glm::vec2 getWorldCoordinates(glm::vec2 deviceCoordinates) const;



	uint32_t layerMask = 0xFFFFFFFF;
};