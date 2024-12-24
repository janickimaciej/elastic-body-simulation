#include "guis/controlPanel.hpp"

#include <imgui/imgui.h>

#include <algorithm>
#include <limits>

ControlPanel::ControlPanel(Scene& scene, Simulation& simulation, const glm::vec2& pos,
	const glm::vec2& size) :
	m_scene{scene},
	m_simulation{simulation},
	m_pos{pos},
	m_size{size}
{ }

void ControlPanel::update()
{
	ImGui::SetNextWindowPos({m_pos.x, m_pos.y}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({m_size.x, m_size.y}, ImGuiCond_Always);
	ImGui::Begin("controlPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

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
		0.0f
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
		[this] () { return m_simulation.getGravity(); },
		[this] (bool gravity) { m_simulation.setGravity(gravity); },
		"gravity"
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
		[this] () { return m_scene.getControlCube().getPos().x; },
		[this] (float x)
		{
			Model& cube = m_scene.getControlCube();
			glm::vec3 pos = cube.getPos();
			pos.x = x;
			cube.setPos(pos);
		},
		"x"
	);

	updateDragFloat
	(
		[this] () { return m_scene.getControlCube().getPos().y; },
		[this] (float y)
		{
			Model& cube = m_scene.getControlCube();
			glm::vec3 pos = cube.getPos();
			pos.y = y;
			cube.setPos(pos);
		},
		"y"
	);

	updateDragFloat
	(
		[this] () { return m_scene.getControlCube().getPos().z; },
		[this] (float z)
		{
			Model& cube = m_scene.getControlCube();
			glm::vec3 pos = cube.getPos();
			pos.z = z;
			cube.setPos(pos);
		},
		"z"
	);

	updateDragFloat
	(
		[this] () { return glm::degrees(m_scene.getControlCube().getPitchRad()); },
		[this] (float pitchDeg)
		{
			Model& cube = m_scene.getControlCube();
			cube.setPitchRad(glm::radians(pitchDeg));
		},
		"pitch",
		1.0f
	);

	updateDragFloat
	(
		[this] () { return glm::degrees(m_scene.getControlCube().getYawRad()); },
		[this] (float yawDeg)
		{
			Model& cube = m_scene.getControlCube();
			cube.setYawRad(glm::radians(yawDeg));
		},
		"yaw",
		1.0f
	);

	updateDragFloat
	(
		[this] () { return glm::degrees(m_scene.getControlCube().getRollRad()); },
		[this] (float rollDeg)
		{
			Model& cube = m_scene.getControlCube();
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

void ControlPanel::updateInputFloat(const std::function<float()>& get,
	const std::function<void(float)>& set, const std::string& name, std::optional<float> min,
	const std::string& format, float step)
{
	static const std::string suffix = "##controlPanelInputFloat";

	ImGui::PushItemWidth(100);

	float value = get();
	float prevValue = value;
	ImGui::InputFloat((name + suffix).c_str(), &value, step, step, format.c_str());
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

void ControlPanel::updateInputInt(const std::function<int()>& get,
	const std::function<void(int)>& set, const std::string& name, std::optional<int> min)
{
	static const std::string suffix = "##controlPanelInputInt";
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

void ControlPanel::updateDragFloat(const std::function<float()>& get,
	const std::function<void(float)>& set, const std::string& name, float velocity)
{
	static const std::string suffix = "##controlPanelDragFloat";

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

void ControlPanel::updateCheckbox(const std::function<bool()>& get,
	const std::function<void(bool)>& set, const std::string& name)
{
	static const std::string suffix = "##controlPanelCheckbox";

	bool value = get();
	bool prevValue = value;
	ImGui::Checkbox((name + suffix).c_str(), &value);
	if (value != prevValue)
	{
		set(value);
	}
}

void ControlPanel::separator()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}
