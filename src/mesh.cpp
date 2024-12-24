#include "mesh.hpp"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
	bool drawLines, bool dynamic) :
	m_drawLines{drawLines}
{
	createVBO(vertices, dynamic);
	m_indexCount = indices.size();
	createEBO(indices);
	createVAO();
}

Mesh::Mesh(Mesh&& mesh) noexcept
{
	m_indexCount = mesh.m_indexCount;
	m_VBO = mesh.m_VBO;
	m_EBO = mesh.m_EBO;
	m_VAO = mesh.m_VAO;
	m_drawLines = mesh.m_drawLines;

	mesh.m_isValid = false;
}

Mesh::~Mesh()
{
	destroyBuffers();
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept
{
	destroyBuffers();

	m_indexCount = mesh.m_indexCount;
	m_VBO = mesh.m_VBO;
	m_EBO = mesh.m_EBO;
	m_VAO = mesh.m_VAO;
	m_drawLines = mesh.m_drawLines;

	mesh.m_isValid = false;

	return *this;
}

void Mesh::update(const std::vector<Vertex>& vertices) const
{
	updateVBO(vertices, true);
}

void Mesh::render() const
{
	glBindVertexArray(m_VAO);
	glDrawElements(m_drawLines ? GL_LINES : GL_TRIANGLES, static_cast<GLsizei>(m_indexCount),
		GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::createVBO(const std::vector<Vertex>& vertices, bool dynamic)
{
	glGenBuffers(1, &m_VBO);
	updateVBO(vertices, dynamic);
}

void Mesh::createEBO(const std::vector<unsigned int>& indices)
{
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(),
		GL_STATIC_DRAW);
}

void Mesh::createVAO()
{
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, pos)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, normalVector)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBindVertexArray(0);
}

void Mesh::updateVBO(const std::vector<Vertex>& vertices, bool dynamic) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
		vertices.data(), dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void Mesh::destroyBuffers() const
{
	if (m_isValid)
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteBuffers(1, &m_VBO);
	}
}
