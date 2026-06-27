#include <monkey3/adventureroom.h>
#include <monkey3/game.h>
#include <iostream>

AdventureRoom::AdventureRoom(Game &game, glm::ivec2 size, glm::ivec4 gameView, glm::ivec4 uiView) :
	Room(game), _size(size), _gameView(gameView), _uiView(uiView) {
	_game.registerToMouseEvent(this);
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

void AdventureRoom::render(const RenderContext& context) {
	Room::render(context);
	if (context.layerMask == 1 && _walkArea) {
		_walkArea->render(_renderer, glm::mat4(1.f));
	}
}

void AdventureRoom::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

}

void AdventureRoom::setCameraPosition(int x, int y) {
	auto& cam = _game.renderer().getRenderPass(0).camera;
	auto halfSize = cam->getSize() * 0.5f;
	cam->setPosition(glm::vec2(x + halfSize.x, y + halfSize.y));
}

void AdventureRoom::cursorPosCallback(GLFWwindow *, double x, double y) {
	auto devCoords = _game.getDeviceCoordinates({x, y});
	//std::cout << "cursor pos callback " << devCoords.x << " " << devCoords.y << std::endl;

	if (_game.isInDeviceCoordinates(devCoords)) {
		//std::cout << "cursor is in device coordinates " << devCoords.x << " " << devCoords.y << std::endl;
	}

	auto c1 = _renderer.getRenderPass(0).getWorldCoordinates(devCoords);
	std::cout << c1.x << " " << c1.y << std::endl;

	// check if cursor is in game view and ui view
	// repeat these steps for each view so ...



}