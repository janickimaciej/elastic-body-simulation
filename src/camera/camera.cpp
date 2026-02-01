#include "camera/camera.hpp"

#include "shaderPrograms.hpp"

#include <glm/gtc/constants.hpp>

Camera::Camera(float aspectRatio, float nearPlane, float farPlane) :
	m_aspectRatio{aspectRatio},
	m_nearPlane{nearPlane},
	m_farPlane{farPlane}
{
	updateViewMatrix();
}

void Camera::use() const
{
	updateShaders();
}

glm::mat4 Camera::getMatrix() const
{
	return m_projectionMatrix * glm::inverse(m_viewMatrixInverse);
}

void Camera::setAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;

	updateProjectionMatrix();
}

void Camera::addPitch(float pitchRad)
{
	m_pitchRad += pitchRad;

	static constexpr float bound = glm::radians(89.0f);
	if (m_pitchRad < -bound)
	{
		m_pitchRad = -bound;
	}
	if (m_pitchRad > bound)
	{
		m_pitchRad = bound;
	}

	updateViewMatrix();
}

void Camera::addYaw(float yawRad)
{
	m_yawRad += yawRad;

	static constexpr float pi = glm::pi<float>();
	while (m_yawRad < -pi)
	{
		m_yawRad += 2 * pi;
	}
	while (m_yawRad >= pi)
	{
		m_yawRad -= 2 * pi;
	}

	updateViewMatrix();
}

void Camera::moveX(float x)
{
	m_targetPos += m_radius * glm::mat3{m_viewMatrixInverse} * glm::vec3{x, 0, 0};

	updateViewMatrix();
}

void Camera::moveY(float y)
{
	m_targetPos += m_radius * glm::mat3{m_viewMatrixInverse} * glm::vec3{0, y, 0};

	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	glm::vec3 pos = getPos();

	glm::vec3 direction = glm::normalize(pos - m_targetPos);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3{0, 1, 0}, direction));
	glm::vec3 up = glm::cross(direction, right);

	m_viewMatrixInverse =
	{
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		direction.x, direction.y, direction.z, 0,
		pos.x, pos.y, pos.z, 1
	};
}

glm::vec3 Camera::getPos() const
{
	return m_targetPos + m_radius *
		glm::vec3
		{
			-std::cos(m_pitchRad) * std::sin(m_yawRad),
			-std::sin(m_pitchRad),
			std::cos(m_pitchRad) * std::cos(m_yawRad)
		};
}

void Camera::updateShaders() const
{
	ShaderPrograms::bezier->use();
	ShaderPrograms::bezier->setUniform("projectionViewMatrix", getMatrix());
	ShaderPrograms::bezier->setUniform("cameraPos", getPos());

	ShaderPrograms::teapot->use();
	ShaderPrograms::teapot->setUniform("projectionViewMatrix", getMatrix());
	ShaderPrograms::teapot->setUniform("cameraPos", getPos());

	ShaderPrograms::lines->use();
	ShaderPrograms::lines->setUniform("projectionViewMatrix", getMatrix());
}
