#include "controlCube.hpp"

#include "elasticCube.hpp"

#include <utility>

ControlCube::ControlCube(const glm::vec3& size)
{
	m_vertices = createVertices(size);
}

std::vector<glm::vec3> ControlCube::getCorners() const
{
	std::vector<glm::vec3> corners{};
	for (const glm::vec3& vertex : m_vertices)
	{
		glm::vec4 affineVertex{vertex, 1};
		corners.push_back({getMatrix() * affineVertex});
	}
	return corners;
}

std::vector<glm::vec3> ControlCube::createVertices(const glm::vec3& size)
{
	std::vector<glm::vec3> vertices(8);
	for (std::size_t zi = 0; zi < 2; ++zi)
	{
		float z = -0.5f + zi;
		for (std::size_t yi = 0; yi < 2; ++yi)
		{
			float y = -0.5f + yi;
			for (std::size_t xi = 0; xi < 2; ++xi)
			{
				float x = -0.5f + xi;
				vertices[index(xi, yi, zi)] = size * glm::vec3{x, y, z};
			}
		}
	}
	return vertices;
}

std::size_t ControlCube::index(std::size_t xi, std::size_t yi, std::size_t zi)
{
	return 4 * zi + 2 * yi + xi;
}
