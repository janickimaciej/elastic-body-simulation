#pragma once

#include "mesh.hpp"

#include <glm/glm.hpp>

#include <array>
#include <string>
#include <string_view>
#include <vector>

class ObjParser
{
public:
	ObjParser() = delete;
	static std::vector<Mesh::Vertex> parse(const std::string& path);
	~ObjParser() = delete;

private:
	static glm::vec3 parsePos(const std::string_view line);
	static glm::vec3 parseNormalVector(const std::string_view line);
	static std::array<Mesh::Vertex, 3> parseTriangle(const std::string_view line,
		const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& normalVectors);
};
