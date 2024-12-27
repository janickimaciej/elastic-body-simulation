#pragma once

#include "model.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

class ElasticCube
{
public:
	ElasticCube(const glm::vec3& size);
	std::vector<glm::vec3> getVertices() const;
	void setVertices(const std::vector<glm::vec3>& vertices);
	std::vector<glm::vec3> getCorners() const;
	
	static std::vector<glm::vec3> createVertices(const glm::vec3& size);
	static std::vector<glm::vec3> createCorners(const glm::vec3& size);
	static std::vector<std::pair<std::size_t, std::size_t>> createSprings();
	static std::vector<std::pair<std::size_t, std::size_t>> createShortSprings();

	static std::vector<std::size_t> cornerIndices();

private:
	std::vector<glm::vec3> m_vertices{};

	static std::size_t index(std::size_t xi, std::size_t yi, std::size_t zi);
};
