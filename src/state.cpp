#include "state.hpp"

State::State(const RungeKutta::State& state)
{
	for (std::size_t i = 0; i < 64; ++i)
	{
		poss[i].x = state[3 * i];
		poss[i].y = state[3 * i + 1];
		poss[i].z = state[3 * i + 2];
		static constexpr std::size_t offset = 3 * 64;
		velocities[i].x = state[offset + 3 * i];
		velocities[i].y = state[offset + 3 * i + 1];
		velocities[i].z = state[offset + 3 * i + 2];
	}
}

RungeKutta::State State::toArray() const
{
	RungeKutta::State state{};
	for (std::size_t i = 0; i < 64; ++i)
	{
		state[3 * i] = poss[i].x;
		state[3 * i + 1] = poss[i].y;
		state[3 * i + 2] = poss[i].z;
		static constexpr std::size_t offset = 3 * 64;
		state[offset + 3 * i] = velocities[i].x;
		state[offset + 3 * i + 1] = velocities[i].y;
		state[offset + 3 * i + 2] = velocities[i].z;
	}
	return state;
}
