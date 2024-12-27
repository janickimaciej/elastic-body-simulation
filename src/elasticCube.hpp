#pragma once

#include "model.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

class ControlCube;

class ElasticCube
{
public:
	ElasticCube(const glm::vec3& size);
	std::vector<glm::vec3> getVertices() const;
	std::vector<glm::vec3> getCorners() const;
	std::vector<std::pair<std::size_t, std::size_t>> getSprings() const;
	
	static std::vector<glm::vec3> createVertices(const glm::vec3& size);
	static std::vector<glm::vec3> createCorners(const glm::vec3& size);
	static std::vector<std::pair<std::size_t, std::size_t>> createSprings();

private:
	std::vector<glm::vec3> m_vertices{};
	std::vector<std::pair<std::size_t, std::size_t>> m_springs{};

	static std::size_t index(std::size_t xi, std::size_t yi, std::size_t zi);
};
