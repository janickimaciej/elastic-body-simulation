#include "model.hpp"

#include <utility>

Model::Model(Mesh mesh, const ShaderProgram& shaderProgram, const glm::vec4& color,
	bool depthOffset) :
	m_mesh{std::move(mesh)},
	m_shaderProgram{shaderProgram},
	m_color{color},
	m_depthOffset{depthOffset}
{ }

void Model::updateMesh(const std::vector<Mesh::Vertex>& vertices)
{
	m_mesh.update(vertices);
}

void Model::render() const
{
	m_shaderProgram.use();
	m_shaderProgram.setUniform("modelMatrix", getMatrix());
	m_shaderProgram.setUniform("color", m_color);
	m_shaderProgram.setUniform("depthOffset", m_depthOffset);
	m_mesh.render();
}

const Mesh& Model::getMesh() const
{
	return m_mesh;
}

const ShaderProgram& Model::shaderProgram() const
{
	return m_shaderProgram;
}
