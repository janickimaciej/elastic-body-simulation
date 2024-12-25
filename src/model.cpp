#include "model.hpp"

#include <utility>

Model::Model(Mesh mesh, const ShaderProgram& shaderProgram, const glm::vec4& color) :
	m_mesh{std::move(mesh)},
	m_shaderProgram{shaderProgram},
	m_color{color}
{ }

void Model::updateMesh(const std::vector<Mesh::Vertex>& vertices)
{
	m_mesh.update(vertices);
}

void Model::render() const
{
	m_shaderProgram.use();
	m_shaderProgram.setUniform("modelMatrix", m_matrix);
	m_shaderProgram.setUniform("color", m_color);
	m_mesh.render();
}

glm::vec3 Model::getPos() const
{
	return m_pos;
}

void Model::setPos(const glm::vec3& pos)
{
	m_pos = pos;
	updateMatrix();
}

float Model::getPitchRad() const
{
	return m_pitchRad;
}

void Model::setPitchRad(float pitchRad)
{
	m_pitchRad = pitchRad;
	updateMatrix();
}

float Model::getYawRad() const
{
	return m_yawRad;
}

void Model::setYawRad(float yawRad)
{
	m_yawRad = yawRad;
	updateMatrix();
}

float Model::getRollRad() const
{
	return m_rollRad;
}

void Model::setRollRad(float rollRad)
{
	m_rollRad = rollRad;
	updateMatrix();
}

glm::quat Model::getOrientation() const
{
	return glm::vec3{m_pitchRad, m_yawRad, m_rollRad};
}

const Mesh& Model::getMesh() const
{
	return m_mesh;
}

glm::mat4 Model::getMatrix() const
{
	return m_matrix;
}

const ShaderProgram& Model::shaderProgram() const
{
	return m_shaderProgram;
}

void Model::updateMatrix()
{
	glm::mat4 orientationMatrix = glm::mat4_cast(getOrientation());

	glm::mat4 positionMatrix
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_pos.x, m_pos.y, m_pos.z, 1
		};

	m_matrix = positionMatrix * orientationMatrix;
}
