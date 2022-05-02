#include "ShaderManager.h"

std::shared_ptr<ShaderManager> ShaderManager::m_Instance;

std::shared_ptr<ShaderManager> ShaderManager::GetInstance()
{
	if (m_Instance == nullptr)
		m_Instance = std::shared_ptr<ShaderManager>(new ShaderManager());

	return m_Instance;
}

bool ShaderManager::AddShader(std::string shaderName, std::shared_ptr<Shader> shader)
{
	auto itr = m_Shaders.find(shaderName);
	if (itr != m_Shaders.end())
		return false;

	m_Shaders.insert(std::make_pair(shaderName, shader));
	return true;
}
std::shared_ptr<Shader> ShaderManager::GetShader(ShaderType shaderType)
{
	auto shaderPtr = m_Shaders.find(ShadersName.at(int(shaderType)));
	if (shaderPtr != m_Shaders.end())
		return shaderPtr->second;
	else
		return nullptr;
}

string ShaderManager::GetShaderName(ShaderType shaderType)
{
	return ShadersName.at(int(shaderType));
}

std::shared_ptr<Shader> ShaderManager::GetShader(std::string shaderName)
{
	auto shaderPtr = m_Shaders.find(shaderName);
	if (shaderPtr != m_Shaders.end())
		return shaderPtr->second;
	else
		return nullptr;
}

void ShaderManager::Initialize()
{
	std::string adsv = ShaderFileNames.at(0);
	std::string adsf = ShaderFileNames.at(1);

	std::string experimentalv = ShaderFileNames.at(2);
	std::string experimentalf = ShaderFileNames.at(3);

	std::string normalMapv = ShaderFileNames.at(4);
	std::string normalMapf = ShaderFileNames.at(5);

	std::string passthroughv = ShaderFileNames.at(6);
	std::string passthroughf = ShaderFileNames.at(7);

	std::string pbrv = ShaderFileNames.at(8);
	std::string pbrf = ShaderFileNames.at(9);

	std::string terrainShaderV = ShaderFileNames.at(10);
	std::string terrainShaderF = ShaderFileNames.at(11);
	std::string ScreenSpacev = ShaderFileNames.at(12);
	std::string ScreenSpacef = ShaderFileNames.at(13);

	std::string colorFilterv = ShaderFileNames.at(14);
	std::string colorFilterf = ShaderFileNames.at(15);

	std::shared_ptr<Shader> ads = ElementFactory::CreateShader(adsv, adsf);
	std::shared_ptr<Shader> experimental = ElementFactory::CreateShader(experimentalv, experimentalf);
	std::shared_ptr<Shader> normalMap = ElementFactory::CreateShader(normalMapv, normalMapf);
	std::shared_ptr<Shader> passthrough = ElementFactory::CreateShader(passthroughv, passthroughf);
	std::shared_ptr<Shader> pbr = ElementFactory::CreateShader(pbrv, pbrf);
	std::shared_ptr<Shader> terrainShader = ElementFactory::CreateShader(terrainShaderV, terrainShaderF);
	std::shared_ptr<Shader> ScreenSpace = ElementFactory::CreateShader(ScreenSpacev, ScreenSpacef);
	std::shared_ptr<Shader> colorFilter = ElementFactory::CreateShader(colorFilterv, colorFilterf);

	AddShader( ShadersName.at(int(ShaderType::ADS)), ads);
	AddShader( ShadersName.at(int(ShaderType::EXPERIMENTAL)), experimental);
	AddShader( ShadersName.at(int(ShaderType::NORMAL_MAP)), normalMap);
	AddShader( ShadersName.at(int(ShaderType::PASS_THROUGH)), passthrough);
	AddShader( ShadersName.at(int(ShaderType::PBR)), pbr);
	AddShader( ShadersName.at(int(ShaderType::TERRAIN)), terrainShader);
	AddShader( ShadersName.at(int(ShaderType::SCREEN_SPACE)), ScreenSpace);
	AddShader( ShadersName.at(int(ShaderType::COLOR_FILTER)), colorFilter);
}
