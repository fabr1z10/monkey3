#include <monkey3/components/playerwalk2d.h>
#include <monkey3/game.h>

PlayerWalk2D::PlayerWalk2D(Game& game, const ControllerInfo& info) : Walk2D(info), _game(game) {

}

void PlayerWalk2D::control() {
	bool left = _game.isKeyDown(GLFW_KEY_LEFT);
	bool right = _game.isKeyDown(GLFW_KEY_RIGHT);
	if (!left && !right) {
		_direction = WalkDirection::NONE;
	}
	else {
		_direction = right ? WalkDirection::RIGHT : WalkDirection::LEFT;
	}

	if (_game.isKeyDown(GLFW_KEY_UP) && grounded()) {
		_v.y = _jumpVelocity;
	}
}