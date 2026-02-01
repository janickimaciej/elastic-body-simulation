#pragma once

#include "scene.hpp"
#include "simulation.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <optional>
#include <string>

class LeftPanel
{
public:
	static constexpr int width = 360;

	LeftPanel(Scene& scene, Simulation& simulation, const glm::ivec2& viewportSize);
	void update();

private:
	Scene& m_scene;
	Simulation& m_simulation;
	const glm::ivec2& m_viewportSize;

	void updateInputFloat(const std::function<float()>& get, const std::function<void(float)>& set,
		const std::string& name, std::optional<float> min = std::nullopt,
		std::optional<float> max = std::nullopt, const std::string& format = "%.1f",
		float step = 0.1f);
	void updateInputInt(const std::function<int()>& get, const std::function<void(int)>& set,
		const std::string& name, std::optional<int> min = std::nullopt);
	void updateDragFloat(const std::function<float()>& get, const std::function<void(float)>& set,
		const std::string& name, float velocity = 0.1f);
	void updateCheckbox(const std::function<bool()>& get, const std::function<void(bool)>& set,
		const std::string& name);
	void separator();

	static void normalizeAngle(float& angleDeg);
};
