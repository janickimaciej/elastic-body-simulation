#pragma once

#include <array>
#include <cstddef>
#include <functional>

class RungeKutta
{
public:
	static constexpr std::size_t stateLength = 7;
	using State = std::array<float, stateLength>;
	using RHS = std::function<State(float, const State&)>;

	RungeKutta() = delete;
	static State RK4(float oldTime, float timeStep, const State& oldState, const RHS& rhs);
	~RungeKutta() = delete;
};
