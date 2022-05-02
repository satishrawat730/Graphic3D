#pragma once

#include "glm/mat4x4.hpp"

class Camera
{

public:
	Camera();
	~Camera();

	Camera(glm::vec3 position,		
		glm::vec3 upVector,			
		float nearPlane,
		float farPlane,
		float yaw,
		float pitch,
		float startMoveSpeed,
		float startTurnSpeed);

	virtual glm::mat4 GetProjectionMatrix() = 0;

	glm::mat4 GetViewMatrix();

	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraFrontVector();
	glm::vec3 GetCameraUpVector();
	glm::vec3 GetCameraRightVector();

	void SetCameraPosition(glm::vec3 cameraPosition);
	void SetCameraFrontVector(glm::vec3 cameraFrontVector);
	void SetCameraUpVector(glm::vec3 cameraUpVector);
	void SetCameraRightVector(glm::vec3 cameraRightVector);

	float GetNearPlane();
	float GetFarPlane();

	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	float GetMoveSpeed();
	void SetMoveSpeed(float moveSpeed);

	float GetTurnSpeed();
	void SetTurnSpeed(float turnSpeed);

	float GetYaw();
	void SetYaw(float yaw);

	float GetPitch();
	void SetPitch(float pitch);

	void Update();

private:

	glm::vec3 m_Position;
	glm::vec3 m_FrontVector;
	glm::vec3 m_UpVector;
	glm::vec3 m_RightVector;
	glm::vec3 m_GlobalUpVector;

	float m_NearPlane;
	float m_FarPlane;

	float m_Yaw;
	float m_Pitch;

	float m_movementSpeed;
	float m_turnSpeed;

protected:

};
