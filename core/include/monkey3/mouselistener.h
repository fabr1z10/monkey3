#pragma once

 struct GLFWwindow;;


class MouseListener {
public:
	MouseListener() = default;

	virtual~ MouseListener() = default;

	virtual void cursorPosCallback(GLFWwindow*, double, double) = 0;

	virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) = 0;
};