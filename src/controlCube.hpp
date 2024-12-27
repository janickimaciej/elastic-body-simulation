#pragma once

#include "frame.hpp"
#include "model.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <vector>

class ControlCube : public Frame
{
public:
	ControlCube(const glm::vec3& size);
	std::vector<glm::vec3> getCorners() const;

	static std::vector<glm::vec3> createVertices(const glm::vec3& size);

private:
	std::vector<glm::vec3> m_vertices{};
	
	static std::size_t index(std::size_t xi, std::size_t yi, std::size_t zi);
};
