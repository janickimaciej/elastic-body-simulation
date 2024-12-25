#include "scene.hpp"

#include "mesh.hpp"

#include <glad/glad.h>

#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

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
	m_constraintBoxModel = std::make_unique<Model>(cubeLineMesh(constraintBoxSize),
		m_linesShaderProgram, constraintBoxColor);

	static constexpr glm::vec4 bezierCubeColor{1, 1, 1, 1};
	//m_bezierCubeModel = std::make_unique<Model>(bezierCubeMesh(cubeSize), m_meshShaderProgram,
	//	bezierCubeColor);

	static constexpr glm::vec4 internalSpringsColor{1, 1, 1, 1};
	m_internalSpringsModel = std::make_unique<Model>(internalSpringsMesh(cubeSize),
		m_linesShaderProgram, internalSpringsColor);

	static constexpr glm::vec4 controlCubeColor{1, 1, 1, 1};
	m_controlCubeModel = std::make_unique<Model>(cubeLineMesh(cubeSize), m_linesShaderProgram,
		controlCubeColor);

	static constexpr glm::vec4 externalSpringsColor{1, 1, 1, 1};
	m_externalSpringsModel = std::make_unique<Model>(externalSpringsMesh(cubeSize),
		m_linesShaderProgram, externalSpringsColor);

	m_elasticCube = std::make_unique<ElasticCube>(cubeSize, *m_bezierCubeModel,
		*m_internalSpringsModel, *m_externalSpringsModel);
	m_controlCube = std::make_unique<ControlCube>(cubeSize, *m_controlCubeModel,
		*m_externalSpringsModel);
	m_elasticCube->init(&*m_controlCube);
	m_controlCube->init(&*m_elasticCube);
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
		m_constraintBoxModel->render();
	}
	if (m_renderBezierCube)
	{
		//m_bezierCubeModel->render();
	}
	if (m_renderInternalSprings)
	{
		m_internalSpringsModel->render();
	}
	if (m_renderControlCube)
	{
		m_controlCubeModel->render();
	}
	if (m_renderExternalSprings)
	{
		m_externalSpringsModel->render();
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
	return *m_controlCubeModel;
}

Mesh Scene::cubeLineMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ControlCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
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

Mesh Scene::bezierCubeMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ElasticCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
	}

	std::vector<unsigned int> indices{}; // TODO

	return Mesh{vertices, indices, false, true};
}

Mesh Scene::internalSpringsMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ElasticCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
	}
	
	std::vector<unsigned int> indices{};
	for (const std::pair<std::size_t, std::size_t>& spring : ElasticCube::createSprings())
	{
		indices.push_back(static_cast<unsigned int>(spring.first));
		indices.push_back(static_cast<unsigned int>(spring.second));
	}

	return Mesh{vertices, indices, true, true};
}

Mesh Scene::externalSpringsMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ControlCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
	}
	for (const glm::vec3& vertexPos : ElasticCube::createCorners(size))
	{
		vertices.push_back({vertexPos, {}});
	}

	std::vector<unsigned int> indices{};
	for (unsigned int i = 0; i < vertices.size() / 2; ++i)
	{
		indices.push_back(i);
		indices.push_back(i + static_cast<unsigned int>(vertices.size()) / 2);
	}

	return Mesh{vertices, indices, true, true};
}

void Scene::setAspectRatio(float aspectRatio)
{
	m_camera.setAspectRatio(aspectRatio);
}
