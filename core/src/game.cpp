#include "monkey3/game.h"
#include <stdexcept>
#include <iostream>
#include <cmath>

Game::Game(const glm::ivec2& deviceSize, const glm::ivec2& windowSize, const std::string &title) :
	_window(nullptr), _deviceSize(deviceSize), _windowSize(windowSize), _title(title) {
	if (_deviceSize.y == 0) {
		throw std::invalid_argument("deviceSize.y cannot be zero");
	}
	_deviceAspectRatio = static_cast<float>(deviceSize.x) / deviceSize.y;
}

Game::~Game() {
	if (_window != nullptr) {
		glfwDestroyWindow(_window);
	}
	glfwTerminate();
}

void Game::init() {
	initGL();

	_renderer.init();
}

void Game::initGL() {
	if(!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW");
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_windowSize.x, _windowSize.y, _title.c_str(), NULL, NULL);
	if( _window == NULL ){
		throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.");
	}
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, Game::windowResizeCallback);
	glfwSetKeyCallback(_window, Game::keyCallback);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);

	std::cout << "OpenGL Version: " << version << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "Vendor: " << vendor << std::endl;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW");
	}
	Game::windowResizeCallback(_window, _windowSize.x, _windowSize.y);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::windowResizeCallback(GLFWwindow *win, int width, int height) {
	auto* game = static_cast<Game*>(glfwGetWindowUserPointer(win));
	if (!game) return;

	game->handleResize(width, height);
}


void Game::handleResize(int width, int height) {
	if (height == 0) {
		height = 1;
	}
	float winAspectRatio = static_cast<float>(width) / height;
	int vx, vy, vw, vh;
	if (winAspectRatio > _deviceAspectRatio) {
		// vertical bands
		vw = (int) (std::round(height * _deviceAspectRatio));
		vh = height;
		vx = (int) ((width - vw) / 2);
		vy = (int) ((height - vh) / 2);
	} else {
		// horizontal bands
		vw = width;
		vh = (int) (width / _deviceAspectRatio);
		vx = 0;
		vy = (int) ((height - vh) / 2);
	}

	_windowViewport = glm::vec4(vx, vy, vw, vh);
}


void Game::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	// TODO
}

void Game::run() {
	if (!_roomFactory) throw std::runtime_error("Room factory not set");
	auto room = _roomFactory->createRoom();
	double last = glfwGetTime();
	while (!glfwWindowShouldClose(_window)) {
		// TO DO
		glfwPollEvents();
		double now = glfwGetTime();
		float dt = now - last;
		last = now;

		room->update(dt);
		room->render(_renderer);
		glfwSwapBuffers(_window);
	}

}

void Game::setRoomFactory(std::unique_ptr<RoomFactory> factory) {
	_roomFactory = std::move(factory);
}