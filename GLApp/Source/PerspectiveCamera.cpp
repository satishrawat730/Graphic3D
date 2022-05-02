#include "PerspectiveCamera.h"
#include <glm/ext/matrix_clip_space.hpp>

PerspectiveCamera::PerspectiveCamera(glm::vec3 position,	
	glm::vec3 upVector,	
	float nearPlane,
	float farPlane,
	float fov,
	float aspectRatio,
	float yaw, 
	float pitch,
	float startMoveSpeed,
	float startTurnSpeed):
	Camera(position, upVector, nearPlane, farPlane, yaw, pitch, startMoveSpeed, startTurnSpeed)
{
	m_FOV = fov;
	m_AspectRatio = aspectRatio;
}

PerspectiveCamera::~PerspectiveCamera()
{

}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(m_FOV), m_AspectRatio, GetNearPlane(), GetFarPlane());
}

float PerspectiveCamera::GetFOV()
{
	return m_FOV;
}

float PerspectiveCamera::GetAspectRatio()
{
	return m_AspectRatio;
}

void PerspectiveCamera::SetFOV(float fov)
{
	m_FOV = fov;
}

void PerspectiveCamera::SetAspectRatio(float asepectRatio)
{
	m_AspectRatio = asepectRatio;
}