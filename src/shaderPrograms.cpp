#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> bezier{};
	std::unique_ptr<const ShaderProgram> teapot{};
	std::unique_ptr<const ShaderProgram> lines{};

	void init()
	{
		bezier = std::make_unique<const ShaderProgram>(path("bezierVS"), path("bezierTCS"),
			path("bezierTES"), path("bezierFS"));
		teapot = std::make_unique<const ShaderProgram>(path("teapotVS"), path("teapotFS"));
		lines = std::make_unique<const ShaderProgram>(path("linesVS"), path("linesFS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}
