#include "rungeKutta.hpp"

RungeKutta::State RungeKutta::RK4(float oldTime, float timeStep, const State& oldState,
	const RHS& rhs)
{
	std::array<State, 4> k{};

	float time{};
	State state{};
	
	k[0] = rhs(oldTime, oldState);

	time = oldTime + timeStep / 2;
	for (std::size_t i = 0; i < stateLength; ++i)
	{
		state[i] = oldState[i] + timeStep / 2 * k[0][i];
	}
	k[1] = rhs(time, state);

	time = oldTime + timeStep / 2;
	for (std::size_t i = 0; i < stateLength; ++i)
	{
		state[i] = oldState[i] + timeStep / 2 * k[1][i];
	}
	k[2] = rhs(time, state);

	time = oldTime + timeStep;
	for (std::size_t i = 0; i < stateLength; ++i)
	{
		state[i] = oldState[i] + timeStep * k[2][i];
	}
	k[3] = rhs(time, state);

	State newState{};
	for (std::size_t i = 0; i < stateLength; ++i)
	{
		newState[i] = oldState[i] + timeStep / 6 * (k[0][i] + 2 * k[1][i] + 2 * k[2][i] + k[3][i]);
	}
	return newState;
}
