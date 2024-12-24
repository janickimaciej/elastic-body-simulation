#include "simulation.hpp"

#include "rungeKutta.hpp"

#include <cmath>

Simulation::Simulation()
{
	start();
}

void Simulation::update()
{
	if (!m_running)
	{
		return;
	}

	float frameT = getSimulationTime();
	int iterations = static_cast<int>(frameT / m_dT);

	while (m_t.size() <= iterations)
	{
		float prevT = (m_t.size() - 1) * m_dT;
		float t = prevT + m_dT;
		m_state = State{RungeKutta::RK4(prevT, m_dT, m_state.toArray(),
			[this] (float, const RungeKutta::State& state)
			{
				return getRHS(state).toArray();
			}
		)};
		m_state.normalize();

		m_t.push_back(t);
	}
}

void Simulation::stop()
{
	m_running = false;
}

void Simulation::start()
{
	if (m_running)
	{
		return;
	}

	m_t.clear();
	m_t.push_back(0);

	resetTime();
	m_running = true;
}

void Simulation::disturb()
{
	// TODO
}

glm::mat3 Simulation::initialRotation()
{
	static constexpr float piOver4 = glm::pi<float>() * 0.25f;

	static const glm::mat3 Ry =
	{
		std::cos(piOver4), 0, -std::sin(piOver4),
		0, 1.0f, 0,
		std::sin(piOver4), 0, std::cos(piOver4)
	};

	static const float angle = std::atan(std::sqrt(2.0f));
	static const glm::mat3 Rz =
	{
		std::cos(angle), std::sin(angle), 0,
		-std::sin(angle), std::cos(angle), 0,
		0, 0, 1.0f
	};

	static const glm::mat3 rotation = Rz * Ry;

	return rotation;
}

float Simulation::getDT() const
{
	return m_dT;
}

void Simulation::setDT(float dT)
{
	if (m_running)
	{
		return;
	}

	m_dT = dT;
}

float Simulation::getMass() const
{
	return m_mass;
}

void Simulation::setMass(float mass)
{
	m_mass = mass;
}

float Simulation::getInternalStiffness() const
{
	return m_internalStiffness;
}

void Simulation::setInternalStiffness(float internalstiffness)
{
	m_internalStiffness = internalstiffness;
}

float Simulation::getExternalStiffness() const
{
	return m_externalStiffness;
}

void Simulation::setExternalStiffness(float externalStiffness)
{
	m_externalStiffness = externalStiffness;
}

float Simulation::getDamping() const
{
	return m_damping;
}

void Simulation::setDamping(float damping)
{
	m_damping = damping;
}

float Simulation::getDisturbanceVelocity() const
{
	return m_disturbanceVelocity;
}

void Simulation::setDisturbanceVelocity(float disturbanceVelocity)
{
	m_disturbanceVelocity = disturbanceVelocity;
}

bool Simulation::getGravity() const
{
	return m_gravity;
}

void Simulation::setGravity(bool gravity)
{
	m_gravity = gravity;
}

int Simulation::getIterations() const
{
	return static_cast<int>(m_t.size());
}

float Simulation::getT() const
{
	if (m_t.empty())
	{
		return 0;
	}
	return m_t.back();
}

glm::quat Simulation::getOrientation() const
{
	return m_state.orientation;
}

float Simulation::getSimulationTime() const
{
	std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
	std::chrono::duration<float> simulationT = t - m_t0;
	return simulationT.count();
}

void Simulation::resetTime()
{
	m_t0 = std::chrono::system_clock::now();
}

State Simulation::getRHS(const State& state) const
{
	State stateDerivative{};

	glm::vec3 torque = {0, 0, 0};
	if (m_gravity)
	{
		static constexpr float g = 9.81f;

		glm::vec3 gravityGlobal{0, -m_mass * g, 0};
		glm::vec3 gravity = glm::conjugate(state.orientation) * gravityGlobal;
		torque = glm::cross(glm::vec3{0, std::sqrt(3.0f) * 0.5f, 0}, gravity);
	}

	stateDerivative.orientation = state.orientation * glm::quat{0, state.angVelocity} * 0.5f;

	stateDerivative.angVelocity = (torque +
		glm::cross(state.angVelocity, state.angVelocity));

	return stateDerivative;
}
