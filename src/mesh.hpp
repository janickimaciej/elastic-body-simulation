#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 pos{};
		glm::vec3 normalVector{};
	};

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
		GLenum drawType = GL_TRIANGLES, bool dynamic = false);
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();
	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&& mesh) noexcept;

	void update(const std::vector<Vertex>& vertices) const;
	void render() const;

private:
	bool m_isValid = true;

	std::size_t m_indexCount{};
	unsigned int m_VBO{};
	unsigned int m_EBO{};
	unsigned int m_VAO{};
	GLenum m_drawType{};

	void createVBO(const std::vector<Vertex>& vertices, bool dynamic);
	void createEBO(const std::vector<unsigned int>& indices);
	void createVAO();
	void updateVBO(const std::vector<Vertex>& vertices, bool dynamic) const;

	void destroyBuffers() const;
};
