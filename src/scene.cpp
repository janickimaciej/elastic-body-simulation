#include "scene.hpp"

#include "mesh.hpp"

#include <glad/glad.h>

#include <cmath>
#include <utility>

static constexpr float fovYDeg = 60.0f;
static constexpr float nearPlane = 0.1f;
static constexpr float farPlane = 1000.0f;

Scene::Scene(const glm::ivec2& viewportSize) :
	m_viewportSize{viewportSize},
	m_camera{fovYDeg, static_cast<float>(viewportSize.x) / viewportSize.y, nearPlane, farPlane,
		m_meshShaderProgram, m_linesShaderProgram}
{
	static constexpr glm::vec3 constraintBoxSize{10.0f, 5.0f, 5.0f};
	static constexpr glm::vec4 constraintBoxColor{1, 1, 1, 1};
	m_constraintBox = std::make_unique<Model>(cubeLineMesh(constraintBoxSize), m_linesShaderProgram,
		constraintBoxColor);
	
	static constexpr glm::vec3 controlCubeSize = {1.0f, 1.0f, 1.0f};
	static constexpr glm::vec4 controlCubeColor{1, 1, 1, 1};
	m_controlCube = std::make_unique<Model>(cubeLineMesh(controlCubeSize), m_linesShaderProgram,
		controlCubeColor);
}

void Scene::update()
{
	m_simulation.update();
}

void Scene::render() const
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.use();

	if (m_renderConstraintBox)
	{
		m_constraintBox->render();
	}
	if (m_renderControlCube)
	{
		m_controlCube->render();
	}
}

void Scene::updateWindowSize()
{
	setAspectRatio(static_cast<float>(m_viewportSize.x) / m_viewportSize.y);
}

void Scene::addPitchCamera(float pitchRad)
{
	m_camera.addPitch(pitchRad);
}

void Scene::addYawCamera(float yawRad)
{
	m_camera.addYaw(yawRad);
}

void Scene::moveXCamera(float x)
{
	m_camera.moveX(x);
}

void Scene::moveYCamera(float y)
{
	m_camera.moveY(y);
}

void Scene::zoomCamera(float zoom)
{
	m_camera.zoom(zoom);
}

bool Scene::getRenderConstraintBox() const
{
	return m_renderConstraintBox;
}

void Scene::setRenderConstraintBox(bool renderConstraintBox)
{
	m_renderConstraintBox = renderConstraintBox;
}

bool Scene::getRenderBezierCube() const
{
	return m_renderBezierCube;
}

void Scene::setRenderBezierCube(bool renderBezierCube)
{
	m_renderBezierCube = renderBezierCube;
}

bool Scene::getRenderInternalSprings() const
{
	return m_renderInternalSprings;
}

void Scene::setRenderInternalSprings(bool renderInternalSprings)
{
	m_renderInternalSprings = renderInternalSprings;
}

bool Scene::getRenderControlCube() const
{
	return m_renderControlCube;
}

void Scene::setRenderControlCube(bool renderControlCube)
{
	m_renderControlCube = renderControlCube;
}

bool Scene::getRenderExternalSprings() const
{
	return m_renderExternalSprings;
}

void Scene::setRenderExternalSprings(bool renderExternalSprings)
{
	m_renderExternalSprings = renderExternalSprings;
}

Simulation& Scene::getSimulation()
{
	return m_simulation;
}

Model& Scene::getControlCube()
{
	return *m_controlCube;
}

Mesh Scene::cubeLineMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices(8);
	vertices[0].pos = {-0.5f, -0.5f, -0.5f};
	vertices[1].pos = {-0.5f, -0.5f, 0.5f};
	vertices[2].pos = {-0.5f, 0.5f, -0.5f};
	vertices[3].pos = {-0.5f, 0.5f, 0.5f};
	vertices[4].pos = {0.5f, -0.5f, -0.5f};
	vertices[5].pos = {0.5f, -0.5f, 0.5f};
	vertices[6].pos = {0.5f, 0.5f, -0.5f};
	vertices[7].pos = {0.5f, 0.5f, 0.5f};
	for (Mesh::Vertex& vertex : vertices)
	{
		vertex.pos *= size;
	}

	std::vector<unsigned int> indices =
	{
		0, 1,
		1, 3,
		3, 2,
		2, 0,
		4, 5,
		5, 7,
		7, 6,
		6, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	return Mesh{vertices, indices, true};
}

void Scene::setAspectRatio(float aspectRatio)
{
	m_camera.setAspectRatio(aspectRatio);
}
