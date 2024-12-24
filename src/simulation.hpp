#pragma once

#include "state.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstddef>
#include <chrono>
#include <functional>
#include <vector>

class Simulation
{
public:
	Simulation();
	void update();
	void stop();
	void start();
	void disturb();

	static glm::mat3 initialRotation();

	float getDT() const;
	void setDT(float dT);
	float getMass() const;
	void setMass(float mass);
	float getInternalStiffness() const;
	void setInternalStiffness(float internalStiffness);
	float getExternalStiffness() const;
	void setExternalStiffness(float externalStiffness);
	float getDamping() const;
	void setDamping(float damping);
	float getDisturbanceVelocity() const;
	void setDisturbanceVelocity(float disturbanceVelocity);
	bool getGravity() const;
	void setGravity(bool gravity);

	int getIterations() const;
	float getT() const;
	glm::quat getOrientation() const;

private:
	bool m_running = false;

	float m_dT = 0.001f;
	float m_mass = 1.0f;
	float m_internalStiffness = 1.0f;
	float m_externalStiffness = 1.0f;
	float m_damping = 1.0f;
	float m_disturbanceVelocity = 1.0f;
	bool m_gravity = true;

	State m_state{};

	std::chrono::time_point<std::chrono::system_clock> m_t0{};
	std::vector<float> m_t{};

	float getSimulationTime() const;
	void resetTime();
	State getRHS(const State& state) const;
};
