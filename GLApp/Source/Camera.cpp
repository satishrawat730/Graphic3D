#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>


Camera::Camera()
{
	m_GlobalUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(glm::vec3 position,
	
	glm::vec3 upVector,		
	float nearPlane,
	float farPlane,
	float yaw,
	float pitch,
	float startMoveSpeed,
	float startTurnSpeed)
{
	m_GlobalUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Position = position;	
	m_GlobalUpVector = upVector;	
	m_NearPlane = nearPlane;
	m_FarPlane = farPlane;
	m_Yaw = yaw;
	m_Pitch = pitch;
	m_movementSpeed = startMoveSpeed;
	m_turnSpeed = startTurnSpeed;

	m_FrontVector = glm::vec3(0.0f, 0.0f, -1.0f);

	Update();

}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMatrix() 
{
	return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

glm::vec3 Camera::GetCameraPosition()
{
	return m_Position;
}

glm::vec3 Camera::GetCameraFrontVector()
{
	return m_FrontVector;
}

glm::vec3 Camera::GetCameraUpVector()
{
	return m_UpVector;
}

glm::vec3 Camera::GetCameraRightVector()
{
	return m_RightVector;
}

void Camera::SetCameraPosition(glm::vec3 cameraPosition)
{
	m_Position = cameraPosition;
}

void Camera::SetCameraFrontVector(glm::vec3 cameraFrontVector)
{
	m_FrontVector = cameraFrontVector;
}

void Camera::SetCameraUpVector(glm::vec3 cameraUpVector)
{
	m_UpVector = cameraUpVector;
}

void Camera::SetCameraRightVector(glm::vec3 cameraRightVector)
{
	m_RightVector = cameraRightVector;
}

float Camera::GetNearPlane()
{
	return m_NearPlane;
}

float Camera::GetFarPlane()
{
	return m_FarPlane;
}

void Camera::SetNearPlane(float nearPlane)
{
	m_NearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	m_FarPlane = farPlane;
}

float Camera::GetMoveSpeed()
{
	return m_movementSpeed;
}

void Camera::SetMoveSpeed(float moveSpeed)
{
	m_movementSpeed = moveSpeed;
}

float Camera::GetTurnSpeed()
{
	return m_turnSpeed;
}

void Camera::SetTurnSpeed(float turnSpeed)
{
	m_turnSpeed = turnSpeed;
}

void Camera::Update()
{
	m_FrontVector.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
	m_FrontVector.y = glm::sin(glm::radians(m_Pitch));
	m_FrontVector.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

	m_FrontVector = glm::normalize(m_FrontVector);

	m_RightVector = glm::cross(m_FrontVector, m_GlobalUpVector);
	m_UpVector = glm::cross(m_RightVector, m_FrontVector);
}

float Camera::GetYaw()
{
	return m_Yaw;
}

void Camera::SetYaw(float yaw)
{
	m_Yaw = yaw;
}

float Camera::GetPitch()
{
	return m_Pitch;
}

void Camera::SetPitch(float pitch)
{
	m_Pitch = pitch;
}