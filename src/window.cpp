#include "window.hpp"

#include <string>

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	static const std::string windowTitle = "cad-opengl";
	m_windowPtr = glfwCreateWindow(m_size.x, m_size.y, windowTitle.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwMakeContextCurrent(m_windowPtr);
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(m_windowPtr, cursorMovementCallback);
	glfwSetScrollCallback(m_windowPtr, scrollCallback);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	glViewport(m_viewportPos.x, m_viewportPos.y, m_viewportSize.x, m_viewportSize.y);
}

Window::~Window()
{
	glfwTerminate();
}

glm::ivec2 Window::viewportSize() const
{
	return m_viewportSize;
}

void Window::setWindowData(Scene& scene, GUI& gui)
{
	m_scene = &scene;
	m_scene->updateWindowSize();
	m_gui = &gui;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_windowPtr);
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(m_windowPtr);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

GLFWwindow* Window::getPtr()
{
	return m_windowPtr;
}

glm::vec2 Window::cursorPos() const
{
	double x{};
	double y{};
	glfwGetCursorPos(m_windowPtr, &x, &y);
	return {static_cast<float>(x), static_cast<float>(y)};
}

void Window::cursorMovementCallback(GLFWwindow* windowPtr, double x, double y)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowPtr));

	glm::vec2 currentPos{static_cast<float>(x), static_cast<float>(y)};
	glm::vec2 offset{currentPos - window->m_lastCursorPos};
	window->m_lastCursorPos = currentPos;

	if ((glfwGetKey(windowPtr, GLFW_KEY_LEFT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ||
		(glfwGetKey(windowPtr, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE &&
		glfwGetKey(windowPtr, GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE &&
		glfwGetKey(windowPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(windowPtr, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE &&
		glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS))
	{
		static constexpr float sensitivity = 0.002f;
		window->m_scene->addPitchCamera(-sensitivity * offset.y);
		window->m_scene->addYawCamera(sensitivity * offset.x);
	}

	if ((glfwGetKey(windowPtr, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
		glfwGetKey(windowPtr, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) &&
		glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		static constexpr float sensitivity = 0.001f;
		window->m_scene->moveXCamera(-sensitivity * offset.x);
		window->m_scene->moveYCamera(sensitivity * offset.y);
	}

	if (glfwGetKey(windowPtr, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS &&
		glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		static constexpr float sensitivity = 1.005f;
		window->m_scene->zoomCamera(std::pow(sensitivity, -offset.y));
	}
}

void Window::scrollCallback(GLFWwindow* windowPtr, double, double yOffset)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowPtr));

	glm::vec2 cursorPos = window->cursorPos();
	if (cursorPos.x <= window->m_viewportPos.x || cursorPos.y <= window->m_viewportPos.y)
	{
		return;
	}

	static constexpr float sensitivity = 1.1f;
	window->m_scene->zoomCamera(std::pow(sensitivity, static_cast<float>(yOffset)));
}
