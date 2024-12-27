#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Frame
{
public:
	virtual ~Frame() = default;

	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);
	float getPitchRad() const;
	void setPitchRad(float pitchRad);
	float getYawRad() const;
	void setYawRad(float yawRad);
	float getRollRad() const;
	void setRollRad(float pitchRad);
	glm::quat getOrientation() const;
	
protected:
	glm::mat4 getMatrix() const;

private:
	glm::mat4 m_matrix{1};

	glm::vec3 m_pos{};
	float m_pitchRad = 0;
	float m_yawRad = 0;
	float m_rollRad = 0;

	void updateMatrix();
};
