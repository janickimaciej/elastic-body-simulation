#include "scene.hpp"

#include "mesh.hpp"
#include "objParser.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

static constexpr float fovYDeg = 60.0f;
static constexpr float nearPlane = 0.1f;
static constexpr float farPlane = 1000.0f;

Scene::Scene(const glm::ivec2& viewportSize) :
	m_viewportSize{viewportSize},
	m_camera{fovYDeg, static_cast<float>(viewportSize.x) / viewportSize.y, nearPlane, farPlane,
		m_bezierShaderProgram, m_teapotShaderProgram, m_linesShaderProgram}
{
	static constexpr glm::vec4 massPointColor{1, 1, 1, 1};
	static constexpr float massPointSize = 0.02f;
	for (int i = 0; i < 64; ++i)
	{
		m_massPointModels.push_back(std::make_unique<Model>(
			cubeMesh(glm::vec3{massPointSize, massPointSize, massPointSize}), m_linesShaderProgram,
			massPointColor));
	}

	static constexpr glm::vec4 constraintBoxColor{0, 0, 1, 1};
	m_constraintBoxModel = std::make_unique<Model>(cubeLineMesh(Simulation::constraintBoxSize),
		m_linesShaderProgram, constraintBoxColor, true);

	static constexpr glm::vec4 bezierCubeColor{0, 1, 0, 0.8f};
	m_bezierCubeModel = std::make_unique<Model>(bezierCubeMesh(Simulation::cubeSize),
		m_bezierShaderProgram, bezierCubeColor);

	static constexpr glm::vec4 teapotColor{1, 1, 1, 1};
	m_teapotModel = std::make_unique<Model>(objMesh("res/teapot.obj"), m_teapotShaderProgram,
		teapotColor);

	static constexpr glm::vec4 internalSpringsColor{1, 1, 1, 1};
	m_internalSpringsModel = std::make_unique<Model>(internalSpringsMesh(Simulation::cubeSize),
		m_linesShaderProgram, internalSpringsColor);

	static constexpr glm::vec4 controlCubeColor{1, 0, 0, 1};
	m_controlCubeModel = std::make_unique<Model>(cubeLineMesh(Simulation::cubeSize),
		m_linesShaderProgram, controlCubeColor, true);

	static constexpr glm::vec4 externalSpringsColor{1, 1, 1, 1};
	m_externalSpringsModel = std::make_unique<Model>(externalSpringsMesh(Simulation::cubeSize),
		m_linesShaderProgram, externalSpringsColor);

	m_simulation = std::make_unique<Simulation>(m_massPointModels, *m_bezierCubeModel,
		*m_internalSpringsModel, *m_controlCubeModel, *m_externalSpringsModel);
}

void Scene::update()
{
	m_simulation->update();
	updateTeapotShader();
}

void Scene::render() const
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.use();

	if (m_renderMassPoints)
	{
		for (const std::unique_ptr<Model>& massPointModel : m_massPointModels)
		{
			massPointModel->render();
		}
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
	if (m_renderTeapot)
	{
		m_teapotModel->render();
	}
	if (m_renderBezierCube)
	{
		m_bezierCubeTexture.use();
		m_bezierCubeModel->render();
	}
	if (m_renderConstraintBox)
	{
		m_constraintBoxModel->render();
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

bool Scene::getRenderMassPoints() const
{
	return m_renderMassPoints;
}

void Scene::setRenderMassPoints(bool renderMassPoints)
{
	m_renderMassPoints = renderMassPoints;
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

bool Scene::getRenderTeapot() const
{
	return m_renderTeapot;
}

void Scene::setRenderTeapot(bool renderTeapot)
{
	m_renderTeapot = renderTeapot;
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
	return *m_simulation;
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

	return Mesh{vertices, indices, GL_LINES};
}

Mesh Scene::cubeMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ControlCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
	}

	std::vector<unsigned int> indices =
	{
		4, 0, 2,
		4, 2, 6,

		1, 5, 7,
		1, 7, 3,

		4, 5, 1,
		4, 1, 0,

		2, 3, 7,
		2, 7, 6,

		5, 4, 6,
		5, 6, 7,

		0, 1, 3,
		0, 3, 2
	};

	return Mesh{vertices, indices, GL_TRIANGLES, false};
}

Mesh Scene::bezierCubeMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ElasticCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
	}

	std::vector<unsigned int> indices{};
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			indices.push_back(12 + 16 * i - 4 * j);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			indices.push_back(3 + 16 * i + 4 * j);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			indices.push_back(3 + 4 * i - j);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			indices.push_back(48 + 4 * i + j);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			indices.push_back(16 * i + j);
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			indices.push_back(15 + 16 * i - j);
		}
	}

	return Mesh{vertices, indices, GL_PATCHES, true};
}

Mesh Scene::internalSpringsMesh(const glm::vec3& size)
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : ElasticCube::createVertices(size))
	{
		vertices.push_back({vertexPos, {}});
	}

	std::vector<unsigned int> indices{};
	for (const std::pair<std::size_t, std::size_t>& spring : ElasticCube::createShortSprings())
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

Mesh Scene::objMesh(const std::string& path)
{
	std::vector<Mesh::Vertex> vertices = ObjParser::parse(path);

	static constexpr float maxFloat = std::numeric_limits<float>::max();
	glm::vec3 minPos{maxFloat, maxFloat, maxFloat};
	glm::vec3 maxPos{-maxFloat, -maxFloat, -maxFloat};

	for (const Mesh::Vertex& vertex : vertices)
	{
		if (vertex.pos.x < minPos.x)
		{
			minPos.x = vertex.pos.x;
		}
		if (vertex.pos.x > maxPos.x)
		{
			maxPos.x = vertex.pos.x;
		}
		if (vertex.pos.y < minPos.y)
		{
			minPos.y = vertex.pos.y;
		}
		if (vertex.pos.y > maxPos.y)
		{
			maxPos.y = vertex.pos.y;
		}
		if (vertex.pos.z < minPos.z)
		{
			minPos.z = vertex.pos.z;
		}
		if (vertex.pos.z > maxPos.z)
		{
			maxPos.z = vertex.pos.z;
		}
	}

	glm::vec3 mean = (minPos + maxPos) / 2.0f;
	glm::vec3 scales = 1.0f / (maxPos - minPos);
	float scale = std::min(scales.x, std::min(scales.y, scales.z));

	for (Mesh::Vertex& vertex : vertices)
	{
		vertex.pos -= mean;
		vertex.pos *= scale;
		vertex.pos += 0.5f;
	}

	std::vector<unsigned int> indices{};
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		indices.push_back(i);
	}

	return Mesh{vertices, indices, GL_TRIANGLES};
}

void Scene::setAspectRatio(float aspectRatio)
{
	m_camera.setAspectRatio(aspectRatio);
}

void Scene::updateTeapotShader() const
{
	m_teapotShaderProgram.use();
	std::vector<glm::vec3> vertices = m_simulation->getElasticCube().getVertices();
	for (int i = 0; i < vertices.size(); ++i)
	{
		m_teapotShaderProgram.setUniform("bezierPoints[" + std::to_string(i) + "]", vertices[i]);
	}
}
