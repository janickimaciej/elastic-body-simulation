#pragma once

#include "frame.hpp"
#include "mesh.hpp"
#include "shaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Model : public Frame
{
public:
	Model(Mesh mesh, const ShaderProgram& shaderProgram, const glm::vec4& color,
		bool depthOffset = false);
	virtual ~Model() = default;

	void updateMesh(const std::vector<Mesh::Vertex>& vertices);
	void render() const;

	const Mesh& getMesh() const;

private:
	Mesh m_mesh;
	const ShaderProgram& m_shaderProgram;
	glm::vec4 m_color{};
	bool m_depthOffset{};

	const ShaderProgram& shaderProgram() const;
};
