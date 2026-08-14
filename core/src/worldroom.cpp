#include <monkey3/worldroom.h>
#include <monkey3/game.h>
#include <iostream>

WorldRoom::WorldRoom(Game& game, glm::ivec2 size, glm::ivec4 gameView, glm::ivec4 uiView) :
	Room(game), _size(size), _gameView(gameView), _uiView(uiView) {

	// setup main pass
	RenderPass mainPass;
	glm::vec2 camSize = glm::vec2(gameView.z, gameView.w);
	auto halfSize = glm::vec2(camSize) * 0.5f;
	mainPass.camera = std::make_unique<OrthoCamera>(gameView.z, gameView.w, 0.1f, 100.f);
	mainPass.viewport = gameView;
	mainPass.layerMask = 1;
	mainPass.camera->setXBounds(halfSize.x, _size.x - halfSize.x);
	mainPass.camera->setYBounds(halfSize.y, _size.y - halfSize.y);
	_game.addRenderPass(std::move(mainPass));

	// setup ui pass
	RenderPass uiPass;
	uiPass.camera = std::make_unique<OrthoCamera>(_uiView.z, _uiView.w, 0.1f, 100.f);
	uiPass.viewport = _uiView;
	uiPass.layerMask = 2;
	_game.addRenderPass(std::move(uiPass));

}