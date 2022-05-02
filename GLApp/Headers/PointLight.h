#pragma once

#include "Light.h"
#include "Mesh.h"
#include "Material.h"
#include "glm/vec3.hpp"

class PointLight : public Light
{
public:
	
	PointLight();
	~PointLight();
	PointLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 position, GLfloat diffuseIntensity,
				float quadratic, float linear, float constant);

	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);

	void UseLight(Shader *shader);

	float GetQudratic();
	void SetQudratic(float val);

	float GetLinear();
	void SetLinear(float val);

	float GetConstant();
	void SetConstant(float val);

	void CreateLightCube(glm::vec3 position);

	std::shared_ptr<Mesh> GetMPointLightMesh() const;
	void SetMPointLightMesh(std::shared_ptr<Mesh> mPointLightMesh);

	std::shared_ptr<Material> GetMLightMaterial() const;
	void SetMLightMaterial(std::shared_ptr<Material> mLightMaterial);

	glm::mat4 GetLightModelMatrix();
	void SetLightModelMatrix(glm::mat4 modelMatrix);

	void DrawLight(glm::mat4 projection, glm::mat4 view);

private:

protected:

	glm::vec3 m_Position;
	float m_constant;
	float m_linear;
	float m_quadratic;
	std::shared_ptr<Mesh> m_PointLightMesh;
	std::shared_ptr<Material> m_LightMaterial;
	glm::mat4 m_LightModel;

};