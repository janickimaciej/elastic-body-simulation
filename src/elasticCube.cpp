#include "elasticCube.hpp"

#include "controlCube.hpp"

#include <utility>

ElasticCube::ElasticCube(const glm::vec3& size)
{
	m_vertices = createVertices(size);
	m_springs = createSprings();
}

std::vector<glm::vec3> ElasticCube::getVertices() const
{
	return m_vertices;
}

std::vector<glm::vec3> ElasticCube::getCorners() const
{
	return
	{
		m_vertices[0], m_vertices[3], m_vertices[12], m_vertices[15],
		m_vertices[48], m_vertices[51], m_vertices[60], m_vertices[63]
	};
}

std::vector<std::pair<std::size_t, std::size_t>> ElasticCube::getSprings() const
{
	return m_springs;
}

std::vector<glm::vec3> ElasticCube::createVertices(const glm::vec3& size)
{
	std::vector<glm::vec3> vertices(64);
	for (std::size_t zi = 0; zi < 4; ++zi)
	{
		float z = -0.5f + zi / 3.0f;
		for (std::size_t yi = 0; yi < 4; ++yi)
		{
			float y = -0.5f + yi / 3.0f;
			for (std::size_t xi = 0; xi < 4; ++xi)
			{
				float x = -0.5f + xi / 3.0f;
				vertices[index(xi, yi, zi)] = size * glm::vec3{x, y, z};
			}
		}
	}
	return vertices;
}

std::vector<glm::vec3> ElasticCube::createCorners(const glm::vec3& size)
{
	std::vector<glm::vec3> vertices = createVertices(size);
	return
	{
		vertices[0], vertices[3], vertices[12], vertices[15],
		vertices[48], vertices[51], vertices[60], vertices[63]
	};
}

std::vector<std::pair<std::size_t, std::size_t>> ElasticCube::createSprings()
{
	std::vector<std::pair<std::size_t, std::size_t>> springs{};
	for (std::size_t zi = 0; zi < 4; ++zi)
	{
		for (std::size_t yi = 0; yi < 4; ++yi)
		{
			for (std::size_t xi = 0; xi < 4; ++xi)
			{
				if (xi < 3)
				{
					springs.push_back({index(xi, yi, zi), index(xi + 1, yi, zi)});
				}
				if (yi < 3)
				{
					springs.push_back({index(xi, yi, zi), index(xi, yi + 1, zi)});
				}
				if (zi < 3)
				{
					springs.push_back({index(xi, yi, zi), index(xi, yi, zi + 1)});
				}
				if (xi < 3 && yi < 3)
				{
					springs.push_back({index(xi, yi, zi), index(xi + 1, yi + 1, zi)});
				}
				if (xi < 3 && yi > 0)
				{
					springs.push_back({index(xi, yi, zi), index(xi + 1, yi - 1, zi)});
				}
				if (yi < 3 && zi < 3)
				{
					springs.push_back({index(xi, yi, zi), index(xi, yi + 1, zi + 1)});
				}
				if (yi < 3 && zi > 0)
				{
					springs.push_back({index(xi, yi, zi), index(xi, yi + 1, zi - 1)});
				}
				if (zi < 3 && xi < 3)
				{
					springs.push_back({index(xi, yi, zi), index(xi + 1, yi, zi + 1)});
				}
				if (zi < 3 && xi > 0)
				{
					springs.push_back({index(xi, yi, zi), index(xi - 1, yi, zi + 1)});
				}
			}
		}
	}
	return springs;
}

std::size_t ElasticCube::index(std::size_t xi, std::size_t yi, std::size_t zi)
{
	return 16 * zi + 4 * yi + xi;
}
