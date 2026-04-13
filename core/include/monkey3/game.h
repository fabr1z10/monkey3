#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "monkey3/roomfactory.h"
#include "monkey3/renderer.h"

class Game {
public:
	Game(const glm::ivec2& deviceSize, const glm::ivec2& windowSize, const std::string& title);

	virtual ~Game();

	Game(const Game&) = delete;

	Game& operator=(const Game&) = delete;

	virtual void init();

	void run();

	void handleResize(int width, int height);

	void setRoomFactory(std::unique_ptr<RoomFactory> factory);
private:
	static void windowResizeCallback(GLFWwindow* win, int width, int height);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);



	void initGL();

	GLFWwindow* _window;

	glm::ivec2 _deviceSize;
	glm::ivec2 _windowSize;
	glm::vec4 _windowViewport;
	float _deviceAspectRatio;
	std::string _title;

	std::unique_ptr<RoomFactory> _roomFactory = nullptr;

	Renderer _renderer;
};