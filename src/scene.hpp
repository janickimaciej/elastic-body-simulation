#pragma once

#include "camera/perspectiveCamera.hpp"
#include "model.hpp"
#include "shaderProgram.hpp"
#include "simulation.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>
#include <vector>

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
	
	std::unique_ptr<Model> m_constraintBox{};
	std::unique_ptr<Model> m_controlCube{};

	bool m_renderConstraintBox = true;
	bool m_renderBezierCube = true;
	bool m_renderInternalSprings = false;
	bool m_renderControlCube = true;
	bool m_renderExternalSprings = false;

	Simulation m_simulation{};

	static Mesh cubeLineMesh(const glm::vec3& size);

	void setAspectRatio(float aspectRatio);
};
