#include "PointLight.h"
#include "ElementFactory.h"
#include "Common.h"
#include "ShaderManager.h"
#include <glm/ext/matrix_transform.hpp>
#include <memory>

PointLight::PointLight()
{
	SetLightType(LightType_PointLight);
}

PointLight::~PointLight()
{

}

PointLight::PointLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 position, GLfloat diffuseIntensity,
						float quadratic, float linear, float constant):
	Light(lightColor, ambientIntensity, diffuseIntensity)
{
	m_Position = position;

	SetLightType(LightType_PointLight);
	m_LightModel = glm::mat4(1.0);
	m_quadratic = quadratic;
	m_linear = linear;
	m_constant = constant;
}

glm::vec3 PointLight::GetPosition()
{
	return m_Position;
}

void PointLight::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void PointLight::UseLight(Shader* shader)
{
	Light::UseLight(shader);
	
	shader->SetValVec3("pointLight.position", m_Position);
	shader->SetValFloat("pointLight.constant", m_constant);
	shader->SetValFloat("pointLight.linear", m_linear);
	shader->SetValFloat("pointLight.quadratic", m_quadratic);
}

float PointLight::GetQudratic()
{
	return m_quadratic;
}

void PointLight::SetQudratic(float val)
{
	m_quadratic = val;
}

float PointLight::GetLinear()
{
	return m_linear;
}

void PointLight::SetLinear(float val)
{
	m_linear = val;
}

float PointLight::GetConstant()
{
	return m_constant;
}

void PointLight::SetConstant(float val)
{
	m_constant = val;
}

void PointLight::CreateLightCube(glm::vec3 position)
{
	unsigned int VAO, VBO, EBO;	

	unsigned int indices[] =
	{
		4,2,0,
		2,7,3,
		6,5,7,
		1,7,5,
		0,3,0,
		4,1,5,
		4,6,2,
		2,6,7,
		6,4,5,
		1,3,7,
		0,2,3,
		4,0,1
	};

	GLfloat vPoints[] = {
			1.000000, 1.000000, -1.000000,
			1.000000, -1.000000, -1.000000,
			1.000000, 1.000000, 1.000000,
			1.000000, -1.000000, 1.000000,
			-1.000000, 1.000000, -1.000000,
			-1.000000, -1.000000, -1.000000,
			-1.000000, 1.000000, 1.000000,
			-1.000000, -1.000000, 1.000000
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPoints), &vPoints, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_PointLightMesh = ElementFactory::CreateMesh(VAO, VBO, EBO, 36);
	BaseColor baseColor;
	baseColor.R = this->GetLightColor().r;
	baseColor.G = this->GetLightColor().g;
	baseColor.B = this->GetLightColor().b;
	m_LightMaterial = ElementFactory::CreateMaterial(baseColor);
	m_LightMaterial->SetShaderName(ShaderManager::GetInstance()->GetShaderName(ShaderType::ADS));
		
	m_LightModel = glm::translate(m_LightModel, m_Position);
	
}
std::shared_ptr<Mesh> PointLight::GetMPointLightMesh() const
{
    return m_PointLightMesh;
}

void PointLight::SetMPointLightMesh(std::shared_ptr<Mesh> mPointLightMesh)
{
    m_PointLightMesh = mPointLightMesh;
}

std::shared_ptr<Material> PointLight::GetMLightMaterial() const
{
    return m_LightMaterial;
}

void PointLight::SetMLightMaterial(std::shared_ptr<Material> mLightMaterial)
{
    m_LightMaterial = mLightMaterial;
}

glm::mat4 PointLight::GetLightModelMatrix()
{
	return m_LightModel;
}

void PointLight::SetLightModelMatrix(glm::mat4 modelMatrix)
{
	m_LightModel = modelMatrix;
}

void PointLight::DrawLight(glm::mat4 projection, glm::mat4 view)
{	
	std::shared_ptr<Shader> shader = ShaderManager::GetInstance()->GetShader(m_LightMaterial->GetShaderName());
	//TODO: Move this to renderer class once it's implemented		
	shader->SetValMat4("model", m_LightModel);
	shader->SetValMat4("projection", projection);
	shader->SetValMat4("view", view);
	shader->SetValVec4("inputColor", glm::vec4(m_LightColor, 1.0));
	glBindVertexArray(m_PointLightMesh->GetVAO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PointLightMesh->GetEBO());		
	glDrawElements(GL_TRIANGLES, m_PointLightMesh->GetTriangleCount(), GL_UNSIGNED_INT, 0);
}