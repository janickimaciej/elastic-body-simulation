#pragma once

#include "camera/camera.hpp"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float fovYDeg, float aspectRatio, float nearPlane, float farPlane);
	float getFOVYDeg() const;
	void setFOVYDeg(float fovYDeg);
	virtual void zoom(float zoom) override;

protected:
	virtual void updateProjectionMatrix() override;

private:
	float m_fovYDeg{};
};
