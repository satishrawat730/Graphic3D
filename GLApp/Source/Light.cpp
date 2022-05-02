#include "Light.h"


Light::Light()
{
	m_AmbientIntensity = 1.0f;
	m_LightColor = glm::vec3(1.0, 1.0, 1.0);	
	m_DiffuseIntensity = 1.0;
}

Light::Light(glm::vec3 lightColor, GLfloat ambientIntensity, GLfloat diffuseIntensity)
{
	SetAmbientIntensity(ambientIntensity);
	SetLightColor(lightColor);	
	SetDiffuseIntensity(diffuseIntensity);
}

Light::~Light() 
{

}

GLfloat Light::GetAmbientIntensity()
{
	return m_AmbientIntensity;
}

void Light::SetAmbientIntensity(GLfloat ambientIntensity)
{
	m_AmbientIntensity = ambientIntensity;
}

glm::vec3 Light::GetLightColor()
{
	return m_LightColor;
}

void Light::SetLightColor(glm::vec3 lightColor)
{
	m_LightColor = lightColor;
}

void Light::UseLight(Shader* shader)
{
	if (m_LightType == LightType_Directional)
	{
		shader->SetValFloat("directionalLight.baseLight.ambientIntensity", m_AmbientIntensity);
		shader->SetValVec3("directionalLight.baseLight.color", m_LightColor);
		shader->SetValFloat("directionalLight.baseLight.diffuseIntensity", m_DiffuseIntensity);
	}
	else if (m_LightType == LightType_PointLight)
	{
		shader->SetValFloat("pointLight.baseLight.ambientIntensity", m_AmbientIntensity);
		shader->SetValVec3("pointLight.baseLight.color", m_LightColor);
		shader->SetValFloat("pointLight.baseLight.diffuseIntensity", m_DiffuseIntensity);
	}	
}

GLfloat Light::GetDiffuseIntensity()
{
	return m_DiffuseIntensity;
}

void Light::SetDiffuseIntensity(GLfloat diffuseIntensity)
{
	m_DiffuseIntensity = diffuseIntensity;
}

LightType Light::GetLightType()
{
	return m_LightType;
}

void Light::SetLightType(LightType lightType)
{
	m_LightType = lightType;
}