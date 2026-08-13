#include <monkey3/keylistener.h>

#include <monkey3/game.h>

KeyboardListener::KeyboardListener(Game& game) : _game(game) {
	_game.registerToKeyboardEvent(this);
}

KeyboardListener::~KeyboardListener() {
	_game.unregisterToKeyboardEvent(this);
}