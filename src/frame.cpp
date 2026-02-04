#include "frame.hpp"

glm::vec3 Frame::getPos() const
{
	return m_pos;
}

void Frame::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateMatrix();
}

float Frame::getPitchRad() const
{
	return m_pitchRad;
}

void Frame::setPitchRad(float pitchRad)
{
	m_pitchRad = pitchRad;
	updateMatrix();
}

float Frame::getYawRad() const
{
	return m_yawRad;
}

void Frame::setYawRad(float yawRad)
{
	m_yawRad = yawRad;
	updateMatrix();
}

float Frame::getRollRad() const
{
	return m_rollRad;
}

void Frame::setRollRad(float rollRad)
{
	m_rollRad = rollRad;
	updateMatrix();
}

glm::quat Frame::getOrientation() const
{
	return glm::vec3{m_pitchRad, m_yawRad, m_rollRad};
}

glm::mat4 Frame::getMatrix() const
{
	return m_matrix;
}

void Frame::updateMatrix()
{
	glm::mat4 orientationMatrix = glm::mat4_cast(getOrientation());

	glm::mat4 posMatrix
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_pos.x, m_pos.y, m_pos.z, 1
		};

	m_matrix = posMatrix * orientationMatrix;
}
