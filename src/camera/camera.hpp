#pragma once

#include <glm/glm.hpp>

#include "shaderProgram.hpp"

class Camera
{
public:
	Camera(float aspectRatio, float nearPlane, float farPlane,
		const ShaderProgram& bezierShaderProgram, const ShaderProgram& teapotShaderProgram,
		const ShaderProgram& linesShaderProgram);
	void use() const;
	glm::mat4 getMatrix() const;
	void setAspectRatio(float aspectRatio);

	void addPitch(float pitchRad);
	void addYaw(float yawRad);
	void moveX(float x);
	void moveY(float y);
	virtual void zoom(float zoom) = 0;

protected:
	float m_aspectRatio{};
	float m_nearPlane{};
	float m_farPlane{};
	float m_radius = 10;

	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};

	float m_projectionPlane = 1;

	void updateViewMatrix();
	virtual void updateProjectionMatrix() = 0;

private:
	glm::vec3 m_targetPos{0, 0, 0};
	float m_pitchRad = 0;
	float m_yawRad = 0;

	const ShaderProgram& m_bezierShaderProgram;
	const ShaderProgram& m_teapotShaderProgram;
	const ShaderProgram& m_linesShaderProgram;

	glm::vec3 getPos() const;
	void updateShaders() const;
};
