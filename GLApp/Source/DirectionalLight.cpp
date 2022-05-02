#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	SetLightType(LightType_Directional);
}

DirectionalLight::~DirectionalLight()
{

}

DirectionalLight::DirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 direction, GLfloat diffuseIntensity):
	Light(lightColor, ambientIntensity, diffuseIntensity)
{
	m_Direction = direction;
	SetLightType(LightType_Directional);
}

glm::vec3 DirectionalLight::GetDirection()
{
	return m_Direction;
}

void DirectionalLight::SetDirection(glm::vec3 direction)
{
	m_Direction = direction;
}

void DirectionalLight::UseLight(Shader* shader)
{
	Light::UseLight(shader);

	shader->SetValVec3("directionalLight.direction", m_Direction);
}

void DirectionalLight::DrawLight(glm::mat4 projection, glm::mat4 view)
{
	//TODO: Implement code to draw rays to show directional light in future
}