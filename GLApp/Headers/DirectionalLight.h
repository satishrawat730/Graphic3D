#pragma once

#include "Light.h"

class DirectionalLight: public Light
{
public: 
	
	DirectionalLight();
	DirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 direction, GLfloat diffuseIntensity);
	~DirectionalLight();

	glm::vec3 GetDirection();
	void SetDirection(glm::vec3 direction);

	void UseLight(Shader* shader);

	void DrawLight(glm::mat4 projection, glm::mat4 view);

private:
	glm::vec3 m_Direction;
protected:
};