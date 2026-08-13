#pragma once

struct GLFWwindow;

class Game;

class KeyboardListener {
public:
	KeyboardListener(Game& game);
	
	virtual~KeyboardListener();
	
	virtual int keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) = 0;

protected:
	Game& _game;
};