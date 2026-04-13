#include "monkey3/renderer.h"
#include "monkey3/utils.h"

void Renderer::init() {

	_quadShader.emplace("assets/shaders/quad.vert", "assets/shaders/quad.frag");
	_lineShader.emplace("assets/shaders/line.vert", "assets/shaders/line.frag");

	initBatches();
}

void Renderer::initBatches() {

}