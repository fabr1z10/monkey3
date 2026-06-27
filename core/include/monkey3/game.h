#pragma once

#include <string>
#include <unordered_set>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "monkey3/roomfactory.h"
#include "monkey3/renderer.h"
#include "monkey3/assetmanager.h"
#include "monkey3/mouselistener.h"

class Game {
public:
	Game(const glm::ivec2& deviceSize, const glm::ivec2& windowSize, const std::string& title);

	virtual ~Game();

	Game(const Game&) = delete;

	Game& operator=(const Game&) = delete;

	virtual void init();

	void run();

	void handleResize(int width, int height);

	void handleMouseMove(double xpos, double ypos);

	void handleMouseButton(int button, int action, int mods);

	void setRoomFactory(std::unique_ptr<RoomFactory> factory);

	void addRenderPass(RenderPass pass);

	//void registerTexture(std::shared_ptr<Tex>);

	AssetManager& assetManager();

	const AssetManager& assetManager() const;

	Renderer& renderer() { return _renderer; }

	void registerToMouseEvent(MouseListener*);

	void unregisterToMouseEvent(MouseListener*);

	// transforms screen coordinate into device coordinates
	glm::vec2 getDeviceCoordinates(glm::vec2);

	bool isInDeviceCoordinates(glm::vec2 s) const;

private:
	static void windowResizeCallback(GLFWwindow* win, int width, int height);

	static void cursor_pos_callback(GLFWwindow*, double xpos, double ypos);

	static void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);



	void initGL();

	GLFWwindow* _window;

	glm::ivec2 _deviceSize;
	glm::ivec2 _windowSize;
	glm::vec4 _windowViewport;
	float _deviceAspectRatio;
	std::string _title;
	int _screenHeight;

	std::unique_ptr<RoomFactory> _roomFactory = nullptr;

	Renderer _renderer;

	AssetManager _assetManager;

	std::unordered_set<MouseListener*> _mouseListeners;

};

inline AssetManager& Game::assetManager() {
	return _assetManager;
}

inline const AssetManager& Game::assetManager() const {
	return _assetManager;
}