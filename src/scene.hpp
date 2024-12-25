#pragma once

#include "camera/perspectiveCamera.hpp"
#include "controlCube.hpp"
#include "elasticCube.hpp"
#include "model.hpp"
#include "shaderProgram.hpp"
#include "simulation.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>

class Scene
{
public:
	Scene(const glm::ivec2& viewportSize);
	void update();
	void render() const;
	void updateWindowSize();

	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void moveXCamera(float x);
	void moveYCamera(float y);
	void zoomCamera(float zoom);

	bool getRenderConstraintBox() const;
	void setRenderConstraintBox(bool renderConstraintBox);
	bool getRenderBezierCube() const;
	void setRenderBezierCube(bool renderBezierCube);
	bool getRenderInternalSprings() const;
	void setRenderInternalSprings(bool renderInternalSprings);
	bool getRenderControlCube() const;
	void setRenderControlCube(bool renderControlCube);
	bool getRenderExternalSprings() const;
	void setRenderExternalSprings(bool renderExternalSprings);

	Simulation& getSimulation();
	Model& getControlCube();

private:
	ShaderProgram m_meshShaderProgram{"src/shaders/meshVS.glsl", "src/shaders/meshFS.glsl"};
	ShaderProgram m_linesShaderProgram{"src/shaders/linesVS.glsl", "src/shaders/linesFS.glsl"};
	glm::ivec2 m_viewportSize{};
	PerspectiveCamera m_camera;
	
	std::unique_ptr<Model> m_constraintBoxModel{};
	std::unique_ptr<Model> m_bezierCubeModel{};
	std::unique_ptr<Model> m_internalSpringsModel{};
	std::unique_ptr<Model> m_controlCubeModel{};
	std::unique_ptr<Model> m_externalSpringsModel{};

	bool m_renderConstraintBox = true;
	bool m_renderBezierCube = true;
	bool m_renderInternalSprings = false;
	bool m_renderControlCube = true;
	bool m_renderExternalSprings = false;
	
	static constexpr glm::vec3 cubeSize = {1.0f, 1.0f, 1.0f};
	std::unique_ptr<ElasticCube> m_elasticCube{};
	std::unique_ptr<ControlCube> m_controlCube{};
	Simulation m_simulation{};

	static Mesh cubeLineMesh(const glm::vec3& size);
	static Mesh bezierCubeMesh(const glm::vec3& size);
	static Mesh internalSpringsMesh(const glm::vec3& size);
	static Mesh externalSpringsMesh(const glm::vec3& size);

	void setAspectRatio(float aspectRatio);
};
