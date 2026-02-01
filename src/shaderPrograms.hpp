#pragma once

#include "shaderProgram.hpp"

#include <memory>

namespace ShaderPrograms
{
	void init();

	extern std::unique_ptr<const ShaderProgram> bezier;
	extern std::unique_ptr<const ShaderProgram> teapot;
	extern std::unique_ptr<const ShaderProgram> lines;
}
