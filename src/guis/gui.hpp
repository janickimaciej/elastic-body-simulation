#pragma once

#include "guis/controlPanel.hpp"
#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class GUI
{
public:
	GUI(GLFWwindow* window, Scene& scene);
	~GUI();

	void update();
	void render();

private:
	Scene& m_scene;
	ControlPanel m_controlPanel;
};
