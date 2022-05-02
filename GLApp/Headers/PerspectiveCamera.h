#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(glm::vec3 position,		
		glm::vec3 upVector,		
		float nearPlane,
		float farPlane,
		float fov,
		float aspectRatio,
		float yaw,
		float pitch,
		float startMoveSpeed,
		float startTurnSpeed);

	~PerspectiveCamera();

	glm::mat4 GetProjectionMatrix();

	float GetFOV();
	float GetAspectRatio();

	void SetFOV(float fov);
	void SetAspectRatio(float asepectRatio);

private:

	float m_FOV;
	float m_AspectRatio;

protected:
};