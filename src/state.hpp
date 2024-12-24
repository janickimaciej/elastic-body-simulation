#pragma once

#include "rungeKutta.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <array>
#include <cstddef>

struct State
{
	static constexpr std::size_t stateLength = 7;

	glm::quat orientation{1, 0, 0, 0};
	glm::vec3 angVelocity{0, 0, 0};

	State() = default;
	State(const RungeKutta::State& state);

	RungeKutta::State toArray() const;
	void normalize();
};
