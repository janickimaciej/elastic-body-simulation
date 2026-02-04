#pragma once

#include "camera/camera.hpp"

#include <glm/glm.hpp>

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(const glm::ivec2& viewportSize, float nearPlane, float farPlane,
		float fovYDeg);

	float getFOVYDeg() const;
	void setFOVYDeg(float fovYDeg);
	virtual void zoom(float zoom) override;

protected:
	virtual void updateProjectionMatrix() override;

private:
	float m_fovYDeg{};
};
