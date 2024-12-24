#pragma once

#include "guis/gui.hpp"
#include "scene.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window();
	~Window();

	glm::ivec2 viewportSize() const;
	void setWindowData(Scene& scene, GUI& gui);
	bool shouldClose() const;
	void swapBuffers() const;
	void pollEvents() const;
	GLFWwindow* getPtr();

private:
	static constexpr glm::ivec2 m_size{1900, 1000};
	static constexpr glm::ivec2 m_viewportPos{360, 0};
	static constexpr glm::ivec2 m_viewportSize = m_size - m_viewportPos;

	GLFWwindow* m_windowPtr{};
	Scene* m_scene{};
	GUI* m_gui{};

	glm::vec2 m_lastCursorPos{};

	glm::vec2 cursorPos() const;

	static void cursorMovementCallback(GLFWwindow* windowPtr, double x, double y);
	static void scrollCallback(GLFWwindow* windowPtr, double, double yOffset);
};
