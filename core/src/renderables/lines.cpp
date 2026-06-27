#include "monkey3/renderables/lines.h"


void Lines::render(Renderer & renderer, glm::mat4 worldTransform) {
	renderer.submitLine(glm::vec2(0, 0), glm::vec2(100, 100), glm::vec4(1.f));
}