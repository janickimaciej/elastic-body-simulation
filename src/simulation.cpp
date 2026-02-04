#include "simulation.hpp"

#include "rungeKutta.hpp"

#include <glm/gtc/random.hpp>

#include <algorithm>
#include <cmath>

Simulation::Simulation(const std::vector<std::unique_ptr<Model>>& massPointModels,
	Model& bezierCubeModel, Model& internalSpringsModel, Model& controlCubeModel,
	Model& externalSpringsModel) :
	m_bezierCubeModel{bezierCubeModel},
	m_internalSpringsModel{internalSpringsModel},
	m_controlCubeModel{controlCubeModel},
	m_externalSpringsModel{externalSpringsModel}
{
	for (const std::unique_ptr<Model>& massPointModel : massPointModels)
	{
		m_massPointModels.push_back(massPointModel.get());
	}

	start();
	std::vector<glm::vec3> vertices = ElasticCube::createVertices(cubeSize);
	std::copy(vertices.begin(), vertices.end(), m_state.poss.begin());
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
		processCollisions();

		m_t.push_back(t);
	}

	updateElasticCube();
	updateModels();
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
	for (glm::vec3& velocity : m_state.velocities)
	{
		float randCoefficient = m_uniformDistribution(m_randomEngine);
		glm::vec3 randomVector{};
		while (randomVector.x == 0 && randomVector.y == 0 && randomVector.z == 0)
		{
			randomVector = glm::vec3{m_normalDistribution(m_randomEngine),
				m_normalDistribution(m_randomEngine), m_normalDistribution(m_randomEngine)};
		}
		glm::vec3 randomDirection = glm::normalize(randomVector);
		velocity += randCoefficient * m_disturbanceVelocity * randomDirection;
	}
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

float Simulation::getCollisionElasticity() const
{
	return m_collisionElasticity;
}

void Simulation::setCollisionElasticity(float collisionElasticity)
{
	m_collisionElasticity = collisionElasticity;
}

float Simulation::getDisturbanceVelocity() const
{
	return m_disturbanceVelocity;
}

void Simulation::setDisturbanceVelocity(float disturbanceVelocity)
{
	m_disturbanceVelocity = disturbanceVelocity;
}

bool Simulation::getExternalSprings() const
{
	return m_externalSprings;
}

void Simulation::setExternalSprings(bool externalsprings)
{
	m_externalSprings = externalsprings;
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

ElasticCube& Simulation::getElasticCube()
{
	return m_elasticCube;
}

ControlCube& Simulation::getControlCube()
{
	return m_controlCube;
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

	for (int i = 0; i < 64; ++i)
	{
		stateDerivative.poss[i] = state.velocities[i];
	}

	std::vector<glm::vec3> internalSpringsForces = getInternalSpringsForces(state);
	std::vector<glm::vec3> externalSpringsForces = getExternalSpringsForces(state);
	std::vector<glm::vec3> dampingForces = getDampingForces(state);
	std::vector<glm::vec3> gravityForces = getGravityForces();

	for (int i = 0; i < 64; ++i)
	{
		glm::vec3 force = internalSpringsForces[i] + dampingForces[i];
		if (m_externalSprings)
		{
			force += externalSpringsForces[i];
		}
		if (m_gravity)
		{
			force += gravityForces[i];
		}
		stateDerivative.velocities[i] = force / particleMass();
	}

	return stateDerivative;
}

void Simulation::updateElasticCube()
{
	std::vector<glm::vec3> vertices(64);
	std::copy(m_state.poss.begin(), m_state.poss.end(), vertices.begin());
	m_elasticCube.setVertices(vertices);
}

void Simulation::updateModels() const
{
	updateMassPointModels();
	updateBezierCubeModel();
	updateInternalSpringsModel();
	updateControlCubeModel();
	updateExternalSpringsModel();
}

void Simulation::updateMassPointModels() const
{
	std::vector<glm::vec3> vertices = m_elasticCube.getVertices();
	for (int i = 0; i < 64; ++i)
	{
		m_massPointModels[i]->setPos(vertices[i]);
	}
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
	m_controlCubeModel.setPos(m_controlCube.getPos());
	m_controlCubeModel.setPitchRad(m_controlCube.getPitchRad());
	m_controlCubeModel.setYawRad(m_controlCube.getYawRad());
	m_controlCubeModel.setRollRad(m_controlCube.getRollRad());
}

void Simulation::updateExternalSpringsModel() const
{
	std::vector<Mesh::Vertex> vertices{};
	for (const glm::vec3& vertexPos : m_controlCube.getCorners())
	{
		vertices.push_back({vertexPos, {}});
	}
	for (const glm::vec3& vertexPos : m_elasticCube.getCorners())
	{
		vertices.push_back({vertexPos, {}});
	}
	m_externalSpringsModel.updateMesh(std::move(vertices));
}

std::vector<glm::vec3> Simulation::getInternalSpringsForces(const State& state) const
{
	std::vector<glm::vec3> forces(64);
	static const std::vector<glm::vec3> referenceVertices = ElasticCube::createVertices(cubeSize);
	for (const std::pair<std::size_t, std::size_t>& spring : m_elasticCube.createSprings())
	{
		float equilibriumLength = glm::length(referenceVertices[spring.second] -
			referenceVertices[spring.first]);
		glm::vec3 springVector = state.poss[spring.second] - state.poss[spring.first];
		glm::vec3 springDirection = glm::normalize(springVector);
		float displacement = glm::length(springVector) - equilibriumLength;
		glm::vec3 force = m_internalStiffness * displacement * springDirection;
		forces[spring.first] += force;
		forces[spring.second] -= force;
	}
	return forces;
}

std::vector<glm::vec3> Simulation::getExternalSpringsForces(const State& state) const
{
	std::vector<glm::vec3> forces(64);
	std::vector<glm::vec3> controlCubeCorners = m_controlCube.getCorners();
	std::vector<std::size_t> cornerIndices = ElasticCube::cornerIndices();
	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 springVector = controlCubeCorners[i] - state.poss[cornerIndices[i]];
		forces[cornerIndices[i]] += m_externalStiffness * springVector;
	}
	return forces;
}

std::vector<glm::vec3> Simulation::getDampingForces(const State& state) const
{
	std::vector<glm::vec3> forces(64);
	for (int i = 0; i < 64; ++i)
	{
		forces[i] = -m_damping * state.velocities[i];
	}
	return forces;
}

std::vector<glm::vec3> Simulation::getGravityForces() const
{
	return std::vector<glm::vec3>(64, glm::vec3{0, -9.81f * particleMass(), 0});
}

void Simulation::processCollisions()
{
	for (int i = 0; i < 64; ++i)
	{
		bool collision = true;
		while (collision)
		{
			collision = false;
			collision |= processCollision(false, -constraintBoxSize.x / 2, m_state.poss[i].x,
				m_state.velocities[i].x);
			collision |= processCollision(true, constraintBoxSize.x / 2, m_state.poss[i].x,
				m_state.velocities[i].x);
			collision |= processCollision(false, -constraintBoxSize.y / 2, m_state.poss[i].y,
				m_state.velocities[i].y);
			collision |= processCollision(true, constraintBoxSize.y / 2, m_state.poss[i].y,
				m_state.velocities[i].y);
			collision |= processCollision(false, -constraintBoxSize.z / 2, m_state.poss[i].z,
				m_state.velocities[i].z);
			collision |= processCollision(true, constraintBoxSize.z / 2, m_state.poss[i].z,
				m_state.velocities[i].z);
		}
	}
}

bool Simulation::processCollision(bool isWallPositive, float wallPos, float& particlePos,
	float& particleVelocity) const
{
	float wallDistance = particlePos - wallPos;
	if (isWallPositive ? wallDistance > 0 : wallDistance < 0)
	{
		particlePos = wallPos - m_collisionElasticity * wallDistance;
		particleVelocity *= -m_collisionElasticity;
		return true;
	}
	return false;
}

float Simulation::particleMass() const
{
	return m_mass / 64;
}
