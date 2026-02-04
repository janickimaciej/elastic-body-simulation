#pragma once

#include "controlCube.hpp"
#include "elasticCube.hpp"
#include "model.hpp"
#include "state.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstddef>
#include <chrono>
#include <functional>
#include <random>
#include <vector>

class Simulation
{
public:
	static constexpr glm::vec3 constraintBoxSize{10.0f, 5.0f, 5.0f};
	static constexpr glm::vec3 cubeSize{1, 1, 1};

	Simulation(const std::vector<std::unique_ptr<Model>>& massPointModels, Model& bezierCubeModel,
		Model& internalSpringsModel, Model& controlCubeModel, Model& externalSpringsModel);
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
	float getCollisionElasticity() const;
	void setCollisionElasticity(float collisionElasticity);
	float getDisturbanceVelocity() const;
	void setDisturbanceVelocity(float disturbanceVelocity);
	bool getExternalSprings() const;
	void setExternalSprings(bool externalSprings);
	bool getGravity() const;
	void setGravity(bool gravity);

	int getIterations() const;
	float getT() const;

	ElasticCube& getElasticCube();
	ControlCube& getControlCube();

private:
	bool m_running = false;

	float m_dT = 0.005f;
	float m_mass = 1.0f;
	float m_internalStiffness = 50.0f;
	float m_externalStiffness = 10.0f;
	float m_damping = 0.03f;
	float m_collisionElasticity = 0.7f;
	float m_disturbanceVelocity = 10.0f;
	bool m_externalSprings = true;
	bool m_gravity = false;

	State m_state{};

	std::chrono::time_point<std::chrono::system_clock> m_t0{};
	std::vector<float> m_t{};

	std::vector<Model*> m_massPointModels{};
	Model& m_bezierCubeModel;
	Model& m_internalSpringsModel;
	Model& m_controlCubeModel;
	Model& m_externalSpringsModel;

	ElasticCube m_elasticCube{cubeSize};
	ControlCube m_controlCube{cubeSize};

	std::random_device m_randomDevice{};
	std::mt19937 m_randomEngine{m_randomDevice()};
	std::uniform_real_distribution<float> m_uniformDistribution{0, 1};
	std::normal_distribution<float> m_normalDistribution{0, 1};

	float getSimulationTime() const;
	void resetTime();
	State getRHS(const State& state) const;

	void updateElasticCube();

	void updateModels() const;
	void updateMassPointModels() const;
	void updateBezierCubeModel() const;
	void updateInternalSpringsModel() const;
	void updateControlCubeModel() const;
	void updateExternalSpringsModel() const;

	std::vector<glm::vec3> getInternalSpringsForces(const State& state) const;
	std::vector<glm::vec3> getExternalSpringsForces(const State& state) const;
	std::vector<glm::vec3> getDampingForces(const State& state) const;
	std::vector<glm::vec3> getGravityForces() const;

	void processCollisions();
	bool processCollision(bool isWallPositive, float wallPos, float& particlePos,
		float& particleVelocity) const;

	float particleMass() const;
};
