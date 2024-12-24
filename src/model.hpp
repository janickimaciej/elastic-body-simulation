#pragma once

#include "mesh.hpp"
#include "shaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Model
{
public:
	Model(Mesh mesh, const ShaderProgram& shaderProgram, const glm::vec4& color);

	virtual void render() const;

	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);
	float getPitchRad() const;
	void setPitchRad(float pitchRad);
	float getYawRad() const;
	void setYawRad(float yawRad);
	float getRollRad() const;
	void setRollRad(float pitchRad);
	glm::quat getOrientation() const;

	const Mesh& getMesh() const;

protected:
	glm::mat4 getMatrix() const;
	const ShaderProgram& shaderProgram() const;

private:
	Mesh m_mesh;
	const ShaderProgram& m_shaderProgram;
	glm::vec4 m_color{};

	glm::mat4 m_matrix{1};

	glm::vec3 m_pos{};
	float m_pitchRad = 0;
	float m_yawRad = 0;
	float m_rollRad = 0;

	void updateMatrix();
};
