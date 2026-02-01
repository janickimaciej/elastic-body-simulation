#pragma once

#include "camera/perspectiveCamera.hpp"
#include "model.hpp"
#include "simulation.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>
#include <string>

class Scene
{
public:
	Scene(const glm::ivec2& viewportSize);
	void update();
	void render() const;
	void updateViewportSize();

	void addPitchCamera(float pitchRad);
	void addYawCamera(float yawRad);
	void moveXCamera(float x);
	void moveYCamera(float y);
	void zoomCamera(float zoom);

	bool getRenderMassPoints() const;
	void setRenderMassPoints(bool renderMassPoints);
	bool getRenderConstraintBox() const;
	void setRenderConstraintBox(bool renderConstraintBox);
	bool getRenderBezierCube() const;
	void setRenderBezierCube(bool renderBezierCube);
	bool getRenderTeapot() const;
	void setRenderTeapot(bool renderTeapot);
	bool getRenderInternalSprings() const;
	void setRenderInternalSprings(bool renderInternalSprings);
	bool getRenderControlCube() const;
	void setRenderControlCube(bool renderControlCube);
	bool getRenderExternalSprings() const;
	void setRenderExternalSprings(bool renderExternalSprings);

	Simulation& getSimulation();

private:
	const glm::ivec2& m_viewportSize{};
	PerspectiveCamera m_camera;

	std::vector<std::unique_ptr<Model>> m_massPointModels{};
	std::unique_ptr<Model> m_constraintBoxModel{};
	std::unique_ptr<Model> m_bezierCubeModel{};
	std::unique_ptr<Model> m_internalSpringsModel{};
	std::unique_ptr<Model> m_controlCubeModel{};
	std::unique_ptr<Model> m_externalSpringsModel{};
	std::unique_ptr<Model> m_teapotModel{};

	Texture m_bezierCubeTexture{"res/sponge.jpg"};

	bool m_renderMassPoints = false;
	bool m_renderConstraintBox = true;
	bool m_renderBezierCube = true;
	bool m_renderTeapot = false;
	bool m_renderInternalSprings = false;
	bool m_renderControlCube = true;
	bool m_renderExternalSprings = false;

	std::unique_ptr<Simulation> m_simulation{};

	static Mesh cubeLineMesh(const glm::vec3& size);
	static Mesh cubeMesh(const glm::vec3& size);
	static Mesh bezierCubeMesh(const glm::vec3& size);
	static Mesh internalSpringsMesh(const glm::vec3& size);
	static Mesh externalSpringsMesh(const glm::vec3& size);
	static Mesh objMesh(const std::string& path);

	void setAspectRatio(float aspectRatio);
	void updateTeapotShader() const;
};
