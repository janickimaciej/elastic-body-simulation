#include "state.hpp"

State::State(const RungeKutta::State& state)
{
	orientation.w = state[0];
	orientation.x = state[1];
	orientation.y = state[2];
	orientation.z = state[3];
	angVelocity.x = state[4];
	angVelocity.y = state[5];
	angVelocity.z = state[6];
}

RungeKutta::State State::toArray() const
{
	RungeKutta::State state{};
	state[0] = orientation.w;
	state[1] = orientation.x;
	state[2] = orientation.y;
	state[3] = orientation.z;
	state[4] = angVelocity.x;
	state[5] = angVelocity.y;
	state[6] = angVelocity.z;
	return state;
}

void State::normalize()
{
	orientation = glm::normalize(orientation);
}
