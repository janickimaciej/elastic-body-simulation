#include "objParser.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>

std::vector<Mesh::Vertex> ObjParser::parse(const std::string& path)
{
	std::ifstream file{path};
	if (!file)
	{
		std::cerr << "File does not exist:\n" << path << '\n';
		assert(false);
		return std::vector<Mesh::Vertex>{};
	}

	std::vector<Mesh::Vertex> vertices{};

	std::vector<glm::vec3> poss{};
	std::vector<glm::vec3> normalVectors{};

	std::string line{};
	while (std::getline(file, line))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			poss.push_back(parsePos(line));
		}
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
		{
			normalVectors.push_back(parseNormalVector(line));
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			std::array<Mesh::Vertex, 3> triangle = parseTriangle(line, poss, normalVectors);
			vertices.push_back(triangle[0]);
			vertices.push_back(triangle[1]);
			vertices.push_back(triangle[2]);
		}
	}

	return vertices;
}

glm::vec3 ObjParser::parsePos(const std::string_view line)
{
	glm::vec3 pos{};

	int component = 0;
	std::string number = "";
	for (auto c = line.begin() + 2; c != line.end(); ++c)
	{
		if (*c == ' ')
		{
			pos[component] = std::stof(number);
			++component;
			number = "";
		}
		else
		{
			number.push_back(*c);
		}
	}
	pos[component] = std::stof(number);

	return pos;
}

glm::vec3 ObjParser::parseNormalVector(const std::string_view line)
{
	glm::vec3 normalVector{};

	int component = 0;
	std::string number = "";
	for (auto c = line.begin() + 3; c != line.end(); ++c)
	{
		if (*c == ' ')
		{
			normalVector[component] = std::stof(number);
			++component;
			number = "";
		}
		else
		{
			number.push_back(*c);
		}
	}
	normalVector[component] = std::stof(number);

	return normalVector;
}

std::array<Mesh::Vertex, 3> ObjParser::parseTriangle(const std::string_view line,
	const std::vector<glm::vec3>& poss, const std::vector<glm::vec3>& normalVectors)
{
	std::array<Mesh::Vertex, 3> triangle;

	std::size_t vertexIndex = 0;
	std::string number = "";
	std::size_t posIndex = 0;
	std::size_t normalVectorIndex = 0;
	bool isFirstNumber = true;
	for (auto c = line.begin() + 2; c != line.end(); ++c)
	{
		if (*c == ' ')
		{
			normalVectorIndex = static_cast<std::size_t>(std::stoi(number));
			number = "";
			if (vertexIndex < 3)
			{
				triangle[vertexIndex].pos = poss[posIndex - 1];
				triangle[vertexIndex].normalVector = normalVectors[normalVectorIndex - 1];
			}
			++vertexIndex;
		}
		else if (*c == '/')
		{
			if (isFirstNumber)
			{
				posIndex = static_cast<std::size_t>(std::stoi(number));
			}
			number = "";
			isFirstNumber = !isFirstNumber;
		}
		else
		{
			number.push_back(*c);
		}
	}
	normalVectorIndex = static_cast<std::size_t>(std::stoi(number));
	if (vertexIndex < 3)
	{
		triangle[vertexIndex].pos = poss[posIndex - 1];
		triangle[vertexIndex].normalVector = normalVectors[normalVectorIndex - 1];
	}

	return triangle;
}
