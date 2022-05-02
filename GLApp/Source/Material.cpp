#include "Material.h"
#include "Texture.h"

Material::Material()
{
	SetShininess(0.0f);
	SetSpecularIntensity(0.0f);
	SetNormalMap(nullptr);
}

Material::~Material()
{

}

Material::Material(BaseColor baseColor, GLfloat specularfactor, GLfloat shininess)
{
	m_BaseColor = baseColor; 
	SetShininess(shininess);
	SetSpecularIntensity(specularfactor);
}

Material::Material(std::shared_ptr<Texture> texture)
{	
	m_DiffuseMap = texture;
	m_BaseColor.R = 1.0;
	m_BaseColor.G = 1.0;
	m_BaseColor.B = 1.0;
}

void Material::SetShininess(GLfloat shininess)
{
	m_Shininess = shininess;
}

GLfloat Material::GetShininess()
{
	return m_Shininess;
}

void Material::SetSpecularIntensity(GLfloat specularFactor)
{
	m_SpecularIntensity = specularFactor;
}

GLfloat Material::GetSpecularFactor()
{
	return m_SpecularIntensity;
}

void Material::UseMaterial(Shader* shader)
{
	shader->SetValFloat("material.shininess", m_Shininess);
	shader->SetValFloat("material.specularIntensity", m_SpecularIntensity);	
	shader->SetValFloat("material.baseColor.R", m_BaseColor.R);
	shader->SetValFloat("material.baseColor.G", m_BaseColor.G);
	shader->SetValFloat("material.baseColor.B", m_BaseColor.B);

	if (m_DiffuseMap != nullptr)
	{
		m_DiffuseMap->UseTexture(GL_TEXTURE0);
		shader->SetValInt("albedoMap", 0);
	}

	if (m_NormalMap != nullptr)
	{
		m_NormalMap->UseTexture(GL_TEXTURE1);
		shader->SetValInt("normalMap", 1);
	}

	if (m_MetallicMap != nullptr)
	{
		m_MetallicMap->UseTexture(GL_TEXTURE2);
		shader->SetValInt("metallicMap", 2);
	}

	if (m_RoughnessMap != nullptr)
	{
		m_RoughnessMap->UseTexture(GL_TEXTURE3);
		shader->SetValInt("roughnessMap", 3);
	}

	if (m_AmbientMap != nullptr)
	{
		m_AmbientMap->UseTexture(GL_TEXTURE4);
		shader->SetValInt("aoMap", 4);
	}

}

BaseColor Material::GetBaseColor()
{
	return m_BaseColor;
}

void Material::SetBaseColor(BaseColor baseColor)
{
	m_BaseColor = baseColor;
}

std::vector<std::shared_ptr<Texture>>& Material::GetDiffuseTextures()
{
	return m_DiffuseTextures;
}

void Material::AddDiffuseTexture(std::shared_ptr<Texture> diffuseTexture)
{
	m_DiffuseTextures.push_back(diffuseTexture);
}

std::shared_ptr<Texture> Material::GetNormalMap()
{
	return m_NormalMap;
}

void Material::SetDiffuseMap(std::shared_ptr<Texture> texture)
{
	m_DiffuseMap = texture;
}

void Material::SetNormalMap(std::shared_ptr<Texture> texture)
{
	m_NormalMap = texture;
}

void Material::SetMetallicMap(std::shared_ptr<Texture> texture)
{
	m_MetallicMap = texture;
}

void Material::SetAmbientMap(std::shared_ptr<Texture> texture)
{
	m_AmbientMap = texture;
}

void Material::SetRoughnessMap(std::shared_ptr<Texture> texture)
{
	m_RoughnessMap = texture;
}

/*
void Material::SetPBRParameters(glm::vec3 albedo, float metallic, float roughness, float ao)
{
	m_PBR.albedo = albedo;
	m_PBR.metallic = metallic;
	m_PBR.roughness = roughness;
	m_PBR.ao = ao;
}

void Material::GetPBRParameters(glm::vec3& in_albedo, float& in_metallic, float& in_roughness, float& in_ao)
{
	in_albedo = m_PBR.albedo;
	in_metallic = m_PBR.metallic;
	in_roughness = m_PBR.roughness;
	in_ao = m_PBR.ao;
}
*/

std::string Material::GetShaderName()
{
	return m_ShaderName;
}

void Material::SetShaderName(std::string shaderName)
{
	m_ShaderName = shaderName;
}