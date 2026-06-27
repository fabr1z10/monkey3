#pragma once

#include "monkey3/room.h"
#include "monkey3/math/walkarea.h"
#include "monkey3/mouselistener.h"


class AdventureRoom : public Room, public MouseListener {
public:
	AdventureRoom(Game& game, glm::ivec2 size, glm::ivec4 gameView, glm::ivec4 uiView);

	void setWalkArea(std::unique_ptr<WalkArea> walkArea) { _walkArea = std::move(walkArea); }

	void render(const RenderContext&) override;

	void cursorPosCallback(GLFWwindow*, double, double) override;

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;

	void setCameraPosition(int x, int y);
private:
	std::unique_ptr<WalkArea> _walkArea;
	glm::ivec2 _size;
	glm::ivec4 _gameView;
	glm::ivec4 _uiView;


};