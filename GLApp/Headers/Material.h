#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>

#include "Texture.h"
#include "Shader.h"
#include "glad/glad.h"

class BaseColor
{
public:
	float R, G, B;
};

class Material
{
public:

	Material();
	~Material();

	//Material(GLfloat specularfactor, GLfloat shininess);

	Material(BaseColor baseColor, GLfloat specularfactor, GLfloat shininess);

	Material(std::shared_ptr<Texture> texture);

	void SetShininess(GLfloat shininess);
	GLfloat GetShininess();

	void SetSpecularIntensity(GLfloat specularFactor);
	GLfloat GetSpecularFactor();

	void UseMaterial(Shader* shader);

	BaseColor GetBaseColor();
	void SetBaseColor(BaseColor baseColor);


	std::shared_ptr<Texture> GetNormalMap();

	void SetDiffuseMap(std::shared_ptr<Texture> texture);

	void SetNormalMap(std::shared_ptr<Texture> texture);

	void SetMetallicMap(std::shared_ptr<Texture> texture);

	void SetAmbientMap(std::shared_ptr<Texture> texture);

	void SetRoughnessMap(std::shared_ptr<Texture> texture);

	std::vector<std::shared_ptr<Texture>>& GetDiffuseTextures();
	void AddDiffuseTexture(std::shared_ptr<Texture> diffuseTexture);

	//void SetPBRParameters(glm::vec3 albedo, float metallic, float roughness, float ao);
	//void GetPBRParameters(glm::vec3& in_albedo, float& in_metallic, float& in_roughness, float& in_ao);
	
	//todo: these are not used for now, 
	std::string GetShaderName();
	void SetShaderName(std::string shaderName);
	
private:

	GLfloat m_Shininess, m_SpecularIntensity;
	BaseColor m_BaseColor;
	std::shared_ptr<Texture> m_DiffuseMap;
	std::shared_ptr<Texture> m_NormalMap;
	std::shared_ptr<Texture> m_MetallicMap; 
	std::shared_ptr<Texture> m_AmbientMap;
	std::shared_ptr<Texture> m_RoughnessMap;


	std::vector<std::shared_ptr<Texture>> m_DiffuseTextures;
	std::string m_ShaderName;
};
