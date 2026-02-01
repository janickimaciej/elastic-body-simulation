#include "gui/leftPanel.hpp"

#include "controlCube.hpp"

#include <imgui/imgui.h>

#include <algorithm>
#include <limits>

LeftPanel::LeftPanel(Scene& scene, Simulation& simulation, const glm::ivec2& viewportSize) :
	m_scene{scene},
	m_simulation{simulation},
	m_viewportSize{viewportSize}
{ }

void LeftPanel::update()
{
	ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({width, static_cast<float>(m_viewportSize.y)}, ImGuiCond_Always);
	ImGui::Begin("leftPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

	if (ImGui::Button("Start"))
	{
		m_simulation.start();
	}

	ImGui::Spacing();

	if (ImGui::Button("Stop"))
	{
		m_simulation.stop();
	}

	ImGui::Spacing();

	ImGui::Text("t = %.2f", m_simulation.getT());

	separator();

	updateInputFloat
	(
		[this] () { return m_simulation.getDT(); },
		[this] (float dt) { m_simulation.setDT(dt); },
		"dt",
		0.001f,
		std::nullopt,
		"%.3f",
		0.001f
	);

	updateInputFloat
	(
		[this] () { return m_simulation.getMass(); },
		[this] (float mass) { m_simulation.setMass(mass); },
		"mass",
		0.1f
	);

	updateInputFloat
	(
		[this] () { return m_simulation.getInternalStiffness(); },
		[this] (float internalStiffness) { m_simulation.setInternalStiffness(internalStiffness); },
		"internal stiffness",
		0.1f
	);

	updateInputFloat
	(
		[this] () { return m_simulation.getExternalStiffness(); },
		[this] (float externalStiffness) { m_simulation.setExternalStiffness(externalStiffness); },
		"external stiffness",
		0.1f
	);

	updateInputFloat
	(
		[this] () { return m_simulation.getDamping(); },
		[this] (float damping) { m_simulation.setDamping(damping); },
		"damping",
		0.0f,
		std::nullopt,
		"%.2f",
		0.01f
	);

	updateInputFloat
	(
		[this] () { return m_simulation.getCollisionElasticity(); },
		[this] (float collisionElasticity)
		{
			m_simulation.setCollisionElasticity(collisionElasticity);
		},
		"collision elasticity",
		0.0f,
		1.0f
	);

	updateInputFloat
	(
		[this] () { return m_simulation.getDisturbanceVelocity(); },
		[this] (float disturbanceVelocity)
		{
			m_simulation.setDisturbanceVelocity(disturbanceVelocity);
		},
		"disturbance velocity",
		0.1f
	);

	separator();

	updateCheckbox
	(
		[this] () { return m_simulation.getExternalSprings(); },
		[this] (bool externalSprings) { m_simulation.setExternalSprings(externalSprings); },
		"external springs"
	);

	updateCheckbox
	(
		[this] () { return m_simulation.getGravity(); },
		[this] (bool gravity) { m_simulation.setGravity(gravity); },
		"gravity"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderMassPoints(); },
		[this] (bool renderMassPoints) { m_scene.setRenderMassPoints(renderMassPoints); },
		"render mass points"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderConstraintBox(); },
		[this] (bool renderConstraintBox) { m_scene.setRenderConstraintBox(renderConstraintBox); },
		"render constraint box"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderBezierCube(); },
		[this] (bool renderBezierCube) { m_scene.setRenderBezierCube(renderBezierCube); },
		"render Bezier cube"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderTeapot(); },
		[this] (bool renderTeapot) { m_scene.setRenderTeapot(renderTeapot); },
		"render teapot"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderInternalSprings(); },
		[this] (bool renderInternalSprings)
		{
			m_scene.setRenderInternalSprings(renderInternalSprings);
		},
		"render internal springs"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderControlCube(); },
		[this] (bool renderControlCube) { m_scene.setRenderControlCube(renderControlCube); },
		"render control cube"
	);

	updateCheckbox
	(
		[this] () { return m_scene.getRenderExternalSprings(); },
		[this] (bool renderExternalSprings)
		{
			m_scene.setRenderExternalSprings(renderExternalSprings);
		},
		"render external springs"
	);

	separator();

	ImGui::Text("Control cube");

	updateDragFloat
	(
		[this] () { return m_scene.getSimulation().getControlCube().getPos().x; },
		[this] (float x)
		{
			ControlCube& cube = m_scene.getSimulation().getControlCube();
			glm::vec3 pos = cube.getPos();
			pos.x = x;
			cube.setPos(pos);
		},
		"x"
	);

	updateDragFloat
	(
		[this] () { return m_scene.getSimulation().getControlCube().getPos().y; },
		[this] (float y)
		{
			ControlCube& cube = m_scene.getSimulation().getControlCube();
			glm::vec3 pos = cube.getPos();
			pos.y = y;
			cube.setPos(pos);
		},
		"y"
	);

	updateDragFloat
	(
		[this] () { return m_scene.getSimulation().getControlCube().getPos().z; },
		[this] (float z)
		{
			ControlCube& cube = m_scene.getSimulation().getControlCube();
			glm::vec3 pos = cube.getPos();
			pos.z = z;
			cube.setPos(pos);
		},
		"z"
	);

	updateDragFloat
	(
		[this] () { return glm::degrees(m_scene.getSimulation().getControlCube().getPitchRad()); },
		[this] (float pitchDeg)
		{
			normalizeAngle(pitchDeg);
			ControlCube& cube = m_scene.getSimulation().getControlCube();
			cube.setPitchRad(glm::radians(pitchDeg));
		},
		"pitch",
		1.0f
	);

	updateDragFloat
	(
		[this] () { return glm::degrees(m_scene.getSimulation().getControlCube().getYawRad()); },
		[this] (float yawDeg)
		{
			normalizeAngle(yawDeg);
			ControlCube& cube = m_scene.getSimulation().getControlCube();
			cube.setYawRad(glm::radians(yawDeg));
		},
		"yaw",
		1.0f
	);

	updateDragFloat
	(
		[this] () { return glm::degrees(m_scene.getSimulation().getControlCube().getRollRad()); },
		[this] (float rollDeg)
		{
			normalizeAngle(rollDeg);
			ControlCube& cube = m_scene.getSimulation().getControlCube();
			cube.setRollRad(glm::radians(rollDeg));
		},
		"roll",
		1.0f
	);

	separator();

	if (ImGui::Button("Disturb"))
	{
		m_simulation.disturb();
	}

	ImGui::End();
}

void LeftPanel::updateInputFloat(const std::function<float()>& get,
	const std::function<void(float)>& set, const std::string& name, std::optional<float> min,
	std::optional<float> max, const std::string& format, float step)
{
	static const std::string suffix = "##leftPanelInputFloat";

	ImGui::PushItemWidth(100);

	float value = get();
	float prevValue = value;
	ImGui::InputFloat((name + suffix).c_str(), &value, step, step, format.c_str());
	if (min.has_value())
	{
		value = std::max(value, *min);
	}
	if (max.has_value())
	{
		value = std::min(value, *max);
	}
	if (value != prevValue)
	{
		set(value);
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updateInputInt(const std::function<int()>& get,
	const std::function<void(int)>& set, const std::string& name, std::optional<int> min)
{
	static const std::string suffix = "##leftPanelInputInt";
	static constexpr int step = 100;

	ImGui::PushItemWidth(100);

	int value = get();
	int prevValue = value;
	ImGui::InputInt((name + suffix).c_str(), &value, step, step);
	if (min.has_value())
	{
		value = std::max(value, *min);
	}
	if (value != prevValue)
	{
		set(value);
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updateDragFloat(const std::function<float()>& get,
	const std::function<void(float)>& set, const std::string& name, float velocity)
{
	static const std::string suffix = "##leftPanelDragFloat";

	ImGui::PushItemWidth(100);

	float value = get();
	float prevValue = value;
	static constexpr float maxFloat = std::numeric_limits<float>::max();
	ImGui::DragFloat((name + suffix).c_str(), &value, velocity, -maxFloat, maxFloat, "%.2f",
		ImGuiSliderFlags_None);
	if (value != prevValue)
	{
		set(value);
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updateCheckbox(const std::function<bool()>& get,
	const std::function<void(bool)>& set, const std::string& name)
{
	static const std::string suffix = "##leftPanelCheckbox";

	bool value = get();
	bool prevValue = value;
	ImGui::Checkbox((name + suffix).c_str(), &value);
	if (value != prevValue)
	{
		set(value);
	}
}

void LeftPanel::separator()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

void LeftPanel::normalizeAngle(float& angleDeg)
{
	while (angleDeg < -180)
	{
		angleDeg += 360;
	}
	while (angleDeg >= 180)
	{
		angleDeg -= 360;
	}
}
