#include "simulation.hpp"

#include "rungeKutta.hpp"

#include <cmath>

Simulation::Simulation(Model& bezierCubeModel, Model& internalSpringsModel, Model& controlCubeModel,
	Model& externalSpringsModel) :
	m_bezierCubeModel{bezierCubeModel},
	m_internalSpringsModel{internalSpringsModel},
	m_controlCubeModel{controlCubeModel},
	m_externalSpringsModel{externalSpringsModel}
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

	// TODO

	return stateDerivative;
}

void Simulation::updateModels() const
{
	updateBezierCubeModel();
	updateInternalSpringsModel();
	updateControlCubeModel();
	updateExternalSpringsModel();
}

void Simulation::updateBezierCubeModel() const
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : m_elasticCube.getVertices())
	{
		vertices.push_back({vertexPos, {}});
	}
	m_bezierCubeModel.updateMesh(std::move(vertices));
}

void Simulation::updateInternalSpringsModel() const
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : m_elasticCube.getVertices())
	{
		vertices.push_back({vertexPos, {}});
	}
	m_internalSpringsModel.updateMesh(std::move(vertices));
}

void Simulation::updateControlCubeModel() const
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : m_controlCube.getVertices())
	{
		vertices.push_back({vertexPos, {}});
	}
	m_controlCubeModel.updateMesh(std::move(vertices));
}

void Simulation::updateExternalSpringsModel() const
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : m_controlCube.getVertices())
	{
		vertices.push_back({vertexPos, {}});
	}
	for (const glm::vec3& vertexPos : m_elasticCube.getCorners())
	{
		vertices.push_back({vertexPos, {}});
	}
	m_externalSpringsModel.updateMesh(std::move(vertices));
}

std::vector<glm::vec3> Simulation::internalSpringsForces()
{
	return {}; // TODO
}

std::vector<glm::vec3> Simulation::externalSpringsForces()
{
	return {}; // TODO
}

std::vector<glm::vec3> Simulation::dampingForces()
{
	return {}; // TODO
}

std::vector<glm::vec3> Simulation::gravityForces()
{
	return std::vector<glm::vec3>(64, glm::vec3{0, -9.81f * m_mass / 64, 0});
}
