#pragma once

#include "rungeKutta.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <array>
#include <cstddef>

struct State
{
	static constexpr std::size_t stateLength = 3 * 128;

	std::array<glm::vec3, 64> positions{};
	std::array<glm::vec3, 64> velocities{};

	State() = default;
	State(const RungeKutta::State& state);

	RungeKutta::State toArray() const;
};
