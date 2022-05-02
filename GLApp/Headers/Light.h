#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Shader.h"

enum LightType
{
	LightType_PointLight,
	LightType_Directional,
	LightType_None
};

class Light
{

public:

	Light();
	Light(glm::vec3 lightColor, GLfloat ambientIntensity, GLfloat diffuseIntensity);

	~Light();

	GLfloat GetAmbientIntensity();
	void SetAmbientIntensity(GLfloat ambientIntensity);

	glm::vec3 GetLightColor();
	void SetLightColor(glm::vec3 lightColor);

	virtual void UseLight(Shader* shader);	

	GLfloat GetDiffuseIntensity();
	void SetDiffuseIntensity(GLfloat diffuseIntensity);

	LightType GetLightType();
	void SetLightType(LightType lightType);

	virtual void DrawLight(glm::mat4 projection, glm::mat4 view) = 0;

private:
	

protected:

	GLfloat m_AmbientIntensity;
	glm::vec3 m_LightColor;
	LightType m_LightType;
	GLfloat m_DiffuseIntensity;

};
